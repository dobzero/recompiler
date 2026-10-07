#include "x86_recipes/x86_functions.h"
#include "x86_registers_allocator.h"
#include "x86_inplace_asm.h"

#include <stdexcept>
#include <format>


class recompiler_exception : public std::runtime_error {
public:
    template <typename ...Args>
    explicit recompiler_exception(const std::format_string<Args...> &fmt_args={}, Args&& ...args) : std::runtime_error(
        std::format(fmt_args, std::forward<Args>(args)...)) {}
};

namespace recompiler::be::x86_64 {
X86InplaceAsm::X86InplaceAsm() : BackendProcessorCaps(SupportedBackends::Arch_X_86_64) {}

}



void recompiler::be::x86_64::X86InplaceAsm::compile_irs(const uint64_t pc_, const std::vector<ir::Ir> &irs_ops, BlockLinking & linker) {

    CompiledStream x86_result;
    const X86RegistersAllocator x86_allocator(X86RegisterType::R14);
    if (pc_!=0) {

    }
    x86_recipes::Functions::do_prologue(x86_result, x86_allocator);

    for (const auto &ir_op : irs_ops) {
        if (ir_op.is_exclusive_of_arch_type)
            if (ir_op.is_exclusive_of_arch_type != type)
                throw recompiler_exception("this ir requires a specific backend");
        switch (ir_op.type) {
            case ir::IrOperationType::Ir_NOP_OP:
                x86_result.write_into<uint8_t>(0x90);
            default: {}
        }
    }

    x86_recipes::Functions::do_epilogue(x86_result);
    compiled_pcs.emplace(pc_, std::move(x86_result));

    const auto cfg_details = enable_jit_in(compiled_pcs[pc_]);
    pc_x_compiled_block.emplace(pc_, cfg_details);
    linker.pc_set_block(pc_, cfg_details.begin); // set next linkable block into linker
}

typedef void (*c_entry_jit_func)(void *state);
size_t recompiler::be::x86_64::X86InplaceAsm::execute_at_pc(const uint64_t pc_, void * thr_addr) {
    if (const auto &pc_x_code_it = pc_x_compiled_block.find(pc_);
        pc_x_code_it!=pc_x_compiled_block.end()) {

        reinterpret_cast<c_entry_jit_func>(pc_x_code_it->second.begin)(thr_addr);
        return pc_x_code_it->second.pc_after;
    }

    return 0;
}
