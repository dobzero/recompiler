#pragma once
#include "types.h"

namespace recompiler::ir {
void ir_shift_right_reg_imm(Ir &ir, uint64_t r, uint64_t imm);

void ir_and_reg_imm(Ir &ir, uint64_t r, uint64_t imm);
void ir_add_reg_imm(Ir &ir, uint64_t r, uint64_t imm);
void ir_branch_offset(Ir &ir, uint64_t offset) ;
void ir_branch_reg(Ir &ir, uint64_t r) ;

void ir_store_reg_imm_reg(Ir &ir, uint64_t r, uint64_t imm, uint64_t r_1);
void ir_load_reg_imm_reg(Ir &ir, uint64_t r, uint64_t imm, uint64_t r_1) ;
void ir_adrof_reg_reg_imm_reg(Ir &ir, uint64_t r, uint64_t r_1, uint64_t imm, uint64_t r_2) ;

void ir_move_reg_reg(Ir &ir, uint64_t r, uint64_t r_1) ;
void ir_move_reg_imm(Ir &ir, uint64_t r, uint64_t imm);
void ir_cmp_reg_reg(Ir &ir, uint64_t r, uint64_t r_1) ;
}
