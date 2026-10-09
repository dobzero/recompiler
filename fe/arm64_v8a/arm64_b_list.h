#pragma once
#include "ir/types.h"

namespace recompiler::fe::arm64_v8a {
    void B_IR(ir::Ir &ir, uint32_t inst);
    using OP_IR = decltype(B_IR);

    struct InstructionMasks{
        uint32_t mask, value;
        OP_IR *or_conv_func_ir;

        size_t hits;
    };
}
