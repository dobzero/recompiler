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

}

void recompiler::be::x86_64::X86InplaceAsm::compile_irs(const uint64_t pc_, const std::vector<ir::Ir> &irs_ops, BlockLinking & linker) {
    constexpr auto to_reg = [](uint64_t val) {
        return static_cast<X86RegisterType>(val);
    };

    CompiledStream x86_cs;
    const X86RegistersAllocator x86_alloc(X86RegisterType::R15);

    if (linker.is_entry_point(pc_))
        x86_recipes::Functions::do_prologue(x86_cs, x86_alloc);

    struct FixBranch {
        ir::Ir ir_val;
        size_t fix_index;
    };
    std::vector<FixBranch> branches_fix_later;
    std::vector<std::pair<const ir::Ir*, size_t>> ir_x_x86_tracing;
    const auto final_pc_of = irs_ops.back().pc;

    for (const auto &ir_op : irs_ops) {
        if (ir_op.is_exclusive_of_arch_type)
            if (ir_op.is_exclusive_of_arch_type != type)
                throw recompiler_exception("this ir requires a specific backend");
        switch (ir_op.type) {
            case ir::IrOperationType::Ir_NOP_OP: {
                x86_recipes::All::NOP(x86_cs);
                break;
            }
            case ir::IrOperationType::Ir_UJUMP_OP: {
                if (ir_op.ir_r_list[0].type==ir::IrOperand::Type::Register) {
                    x86_recipes::All::JMP_REG(x86_cs, to_reg(ir_op.ir_r_list[0].value));
                    break;
                }
                x86_recipes::DataFlow::MOV_R64_MEM(x86_cs, X86RegisterType::RDI, x86_alloc.get_thr_reg(), 0);
                x86_recipes::DataFlow::MOV_R64_IMM64(x86_cs, X86RegisterType::RSI, ir_op.ir_r_list[0].value);
                x86_recipes::DataFlow::MOV_R64_IMM64(x86_cs, X86RegisterType::RAX, reinterpret_cast<uint64_t>(linker.c_stub_function));
                x86_recipes::Functions::CALL(x86_cs, X86RegisterType::RAX);
                x86_recipes::All::JMP_REG(x86_cs, X86RegisterType::RAX);
                break;

            }
            case ir::IrOperationType::Ir_SHR_OP:
                x86_recipes::All::SHR_A64_IMM(x86_cs, to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            case ir::IrOperationType::Ir_AND_OP:
                x86_recipes::All::AND_R64_IMM(x86_cs, to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            case ir::IrOperationType::Ir_ADD_OP:
                x86_recipes::All::ADD_R64_IMM(x86_cs, to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            case ir::IrOperationType::Ir_MUL_OP:
                x86_recipes::All::IMUL_R64_IMM(x86_cs, to_reg(ir_op.ir_r_list[2].value), to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            case ir::IrOperationType::Ir_ADROF_OP:
                x86_recipes::All::LEA_R64_IMM(x86_cs, to_reg(ir_op.ir_r_list[3].value), to_reg(ir_op.ir_r_list[0].value), to_reg(ir_op.ir_r_list[1].value), 1, ir_op.ir_r_list[2].value);
                break;
            case ir::IrOperationType::Ir_MOVE_OP: {
                if (ir_op.ir_r_list[1].type==ir::IrOperand::Type::Immediate) {
                    x86_recipes::DataFlow::MOV_R64_IMM64(x86_cs, to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                } else {
                    x86_recipes::DataFlow::MOV_R64_R64(x86_cs, to_reg(ir_op.ir_r_list[1].value), to_reg(ir_op.ir_r_list[0].value));
                }
                break;
            }
            case ir::IrOperationType::Ir_STORE_OP: {
                x86_recipes::DataFlow::MOV_MEM_R64(x86_cs, to_reg(ir_op.ir_r_list[2].value), to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            }
            case ir::IrOperationType::Ir_LOAD_OP: {
                x86_recipes::DataFlow::MOV_R64_MEM(x86_cs, to_reg(ir_op.ir_r_list[2].value), to_reg(ir_op.ir_r_list[0].value), ir_op.ir_r_list[1].value);
                break;
            }

            default: {
                throw recompiler_exception("ir {} not impl. in this backend", to_string(ir_op.type));
            }
        }

        ir_x_x86_tracing.emplace_back(std::make_pair(&ir_op, x86_cs.get_stream_size()));
    }

    if (linker.is_entry_point(pc_))
        x86_recipes::Functions::do_epilogue(x86_cs);

    compiled_pcs.emplace(pc_, std::move(x86_cs));

    const auto cfg_details = enable_jit_in(compiled_pcs[pc_], irs_ops.back().pc);
    for (const auto &[ir_val, fix_index] : branches_fix_later) {
        const int64_t offset = static_cast<int64_t>(ir_val.ir_r_list.begin()->value);
        const auto target_pc = ir_val.pc+offset;
        const uint8_t * value = [&] {
            if (target_pc>=final_pc_of) { // outside this cfg
                return static_cast<uint8_t*>(linker.get_cfg_jump_addr(ir_val).first);
            }
            const auto &[_, pc_ir] = ir_x_x86_tracing[offset/4];
            return static_cast<uint8_t*>(cfg_details.begin) + pc_ir;
        }();
        memcpy(static_cast<uint8_t *>(cfg_details.begin) + fix_index, &value, 8);
    }
    linker.set_first_block(cfg_details.begin, cfg_details.size);

    pc_x_compiled_block.emplace(pc_, cfg_details);
    linker.pc_set_block(pc_, cfg_details.begin); // set next linkable block into linker
}

typedef void (*c_entry_jit_func)(void *state);

namespace recompiler::be {
ExecutionResult x86_64::X86InplaceAsm::execute_at_pc(const uint64_t pc_, void * thr_addr) {
    if (const auto &pc_x_code_it = pc_x_compiled_block.find(pc_);
        pc_x_code_it!=pc_x_compiled_block.end()) {
        reinterpret_cast<c_entry_jit_func>(pc_x_code_it->second.begin)(thr_addr);
        return {
            .g_pc_begin = pc_,
            .g_pc_end = pc_x_code_it->second.g_pc_after,
        };
    }
    return {};
}
}
