#include "ir_builder.h"

namespace recompiler::ir {
void ir_shift_right_reg_imm(Ir &ir, uint64_t r, uint64_t imm) {
    ir.type = IrOperationType::Ir_SHIFT_RIGHT_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
}

void ir_and_reg_imm(Ir &ir, uint64_t r, uint64_t imm) {
    ir.type = IrOperationType::Ir_AND_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
}

void ir_add_reg_imm(Ir &ir, uint64_t r, uint64_t imm) {
    ir.type = IrOperationType::Ir_ADD_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
}

void ir_branch_offset(Ir &ir, uint64_t offset) {
    ir.type = IrOperationType::Ir_BRANCH_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Offset, offset);
}

void ir_branch_reg(Ir &ir, uint64_t r) {
    ir.type = IrOperationType::Ir_BRANCH_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
}

void ir_store_reg_imm_reg(Ir &ir, uint64_t r, uint64_t imm, uint64_t r_1) {
    ir.type = IrOperationType::Ir_STORE_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_1);
}
void ir_load_reg_imm_reg(Ir &ir, uint64_t r, uint64_t imm, uint64_t r_1) {
    ir.type = IrOperationType::Ir_LOAD_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_1);
}

void ir_adrof_reg_reg_imm_reg(Ir &ir, uint64_t r, uint64_t r_1, uint64_t imm, uint64_t r_2) {
    ir.type = IrOperationType::Ir_ADROF_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_1);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_2);
}

void ir_move_reg_reg(Ir &ir, uint64_t r, uint64_t r_1) {
    ir.type = IrOperationType::Ir_MOVE_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_1);
}
void ir_move_reg_imm(Ir &ir, uint64_t r, uint64_t imm) {
    ir.type = IrOperationType::Ir_MOVE_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, imm);
}

void ir_cmp_reg_reg(Ir &ir, uint64_t r, uint64_t r_1) {
    ir.type = IrOperationType::Ir_CMP_OP;
    ir.ir_r_list.emplace_back(Ir_Type::Register, r);
    ir.ir_r_list.emplace_back(Ir_Type::Register, r_1);
}
}