#include "x86_registers_allocator.h"
#include "x86_recipes/x86_all.h"
#include "x86_inplace_asm.h"

#include "fe/arm64_v8a/processor_arm64_v8a.h"
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

struct CompilationContext {
    CompiledStream x86_cs;
    X86RegistersAllocator x86_alloc{X86RegisterType::R14};
};
}
void recompiler::be::x86_64::X86InplaceAsm::compile_irs(const uint64_t pc_, const std::vector<ir::Ir> &irs_ops, BlockLinking & linker) {

    CompilationContext compiler_ctx{};
    auto &x86_result=compiler_ctx.x86_cs;

    x86_recipes::Functions::do_prologue(compiler_ctx.x86_cs, compiler_ctx.x86_alloc);
    struct FixBranch {
        ir::Ir ir_val;
        size_t fix_index;
    };
    std::vector<FixBranch> branches_fix_later;
    std::vector<std::pair<const ir::Ir*, size_t>> ir_x_x86_tracing;
    const auto final_pc_of = irs_ops[irs_ops.size() - 2].pc;

    for (const auto &ir_op : irs_ops) {
        if (ir_op.is_exclusive_of_arch_type)
            if (ir_op.is_exclusive_of_arch_type != type)
                throw recompiler_exception("this ir requires a specific backend");
        switch (ir_op.type) {
            case ir::IrOperationType::Ir_NOP_OP: {
                x86_recipes::All::NOP(compiler_ctx.x86_cs);
                break;
            }
            case ir::IrOperationType::Ir_BRANCH_OP: {
            }
            case ir::IrOperationType::Ir_RET_OP: {
            }
            default: {}
        }

        ir_x_x86_tracing.emplace_back(std::make_pair(&ir_op, x86_result.get_stream_size()));
    }

    x86_recipes::Functions::do_epilogue(x86_result);
    compiled_pcs.emplace(pc_, std::move(x86_result));

    const auto cfg_details = enable_jit_in(compiled_pcs[pc_]);
    for (const auto &[ir_val, fix_index] : branches_fix_later) {
        const int64_t offset = static_cast<int64_t>(ir_val.ir_r_list.begin()->value);
        const auto target_pc = ir_val.pc+offset;
        const uint8_t * value = [&] {
            if (target_pc>=final_pc_of) { // outside this cfg
                return static_cast<uint8_t*>(linker.get_cfg_jump_addr(ir_val).first);
            }
            const auto &[_, offset_pc_ir] = ir_x_x86_tracing[offset/4];
            return static_cast<uint8_t*>(cfg_details.begin) + offset_pc_ir;
        }();
        memcpy(static_cast<uint8_t *>(cfg_details.begin) + fix_index, &value, 8);
    }

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
