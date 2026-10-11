#pragma once
#include "ir/types.h"

namespace recompiler::fe::arm64_v8a {
    void a_b_ir(inplace_vector<ir::Ir, 15> &ir, uint32_t inst, uint64_t);
    void a_ret_ir(inplace_vector<ir::Ir, 15> &ir, uint32_t inst, uint64_t);
    using OP_IR = decltype(a_b_ir);

    struct InstructionMeta{
        uint32_t mask, value;
        OP_IR *conv_func_ir_callback;

        size_t hits;
    };
}
