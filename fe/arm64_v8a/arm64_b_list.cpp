#include "arm64_b_list.h"

#include "processor_arm64_v8a.h"
#include "ir/ir_builder.h"

namespace recompiler::fe::arm64_v8a {
using A64_S = Arm64ProcessorState;
void a_b_ir(inplace_vector<ir::Ir, 15> &ir, const uint32_t inst, const uint64_t pc) {
    uint64_t offset = 0;
    if (const uint32_t imm26 = inst & 0x03FFFFFF; imm26 & 0x02000000)
        offset |= static_cast<int64_t>(imm26 | 0xFC000000);
    else offset |= imm26;

    offset *= 4;
    ir.emplace_n_and_call([&](const std::span<ir::Ir> &ir_v) {
        // save lr to pc + 4 (our ret addr)
        ir_move_reg_imm(ir_v[0], 1, pc + 4);
        ir_store_reg_imm_reg(ir_v[1], 15, A64_S::offset_of(AliasRegisters::Linker), 1); // THREAD->LINKER = pc + 4
        // set pc to offset
        ir_move_reg_imm(ir_v[2], 1, offset);
        ir_store_reg_imm_reg(ir_v[3], 15, A64_S::offset_of(AliasRegisters::PC), 1); // // THREAD->PC = OFFSET
        // set branch
        ir_branch_offset(ir_v[4], offset);
    }, 5);
}

void a_ret_ir(inplace_vector<ir::Ir, 15> &ir, [[maybe_unused]] const uint32_t inst, [[maybe_unused]] const uint64_t pc) {
    ir.emplace_n_and_call([&](const std::span<ir::Ir> &ir_v) {
        ir_load_reg_imm_reg(ir_v[0], 15, A64_S::offset_of(AliasRegisters::Linker), 0); // $0 = THREAD->Linker
        ir_move_reg_reg(ir_v[1], 0, 2); // $2 = $0
        ir_shift_right_reg_imm(ir_v[2], 0, 2); // $0 =>> 2
        ir_and_reg_imm(ir_v[3], 0, 0x3FF); // $0 =& 0x3FF
        ir_adrof_reg_reg_imm_reg(ir_v[4], 15, 0, A64_S::offset_of(AliasRegisters::FastTable), 1);
        ir_load_reg_imm_reg(ir_v[5], 1, 0, 0);
        ir_cmp_reg_reg(ir_v[6], 0, 2); // if $0 == $2
        ir_store_reg_imm_reg(ir_v[7], 15, A64_S::offset_of(AliasRegisters::PC), 0);
        ir_add_reg_imm(ir_v[8], 1, 8);
        ir_load_reg_imm_reg(ir_v[9], 1, 0, 0);
        ir_branch_reg(ir_v[10], 0);
    }, 11);
}
}
