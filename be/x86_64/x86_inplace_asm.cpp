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

void recompiler::be::x86_64::X86InplaceAsm::compile_irs(const uint64_t pc_, const std::vector<ir::Ir> &irs_ops) {

    CompiledStream x86_result;
    if (pc_!=0) {

    }
    x86_result.write_into<uint8_t>(0x55); //push %rbp
    x86_result.write_bytes({0x48, 0x89, 0xE5}); // mvo %rsp, %rbp
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

    x86_result.write_into<uint8_t>(0xC9); // leave
    x86_result.write_into<uint8_t>(0xC3); // ret
    compiled_pcs.emplace(pc_, std::move(x86_result));
    pc_x_compiled_block.emplace(pc_, enable_jit_in(compiled_pcs[pc_]));
}

typedef void (*c_entry_jit_func)();
size_t recompiler::be::x86_64::X86InplaceAsm::execute_at_pc(const uint64_t pc_) {
    if (const auto &pc_x_code_it = pc_x_compiled_block.find(pc_);
        pc_x_code_it!=pc_x_compiled_block.end()) {

        reinterpret_cast<c_entry_jit_func>(pc_x_code_it->second.begin)();
        return pc_x_code_it->second.pc_after;
    }

    return 0;
}
