#pragma once
#include "ir/types.h"

namespace recompiler::fe::arm64_v8a {
    void B_IR(inplace_vector<ir::Ir, 10> &ir, uint32_t inst, uint64_t);
    using OP_IR = decltype(B_IR);

    struct InstructionMeta{
        uint32_t mask, value;
        OP_IR *or_conv_func_ir;

        size_t hits;
    };
}
