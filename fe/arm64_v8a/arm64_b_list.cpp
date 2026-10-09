
#include "arm64_b_list.h"

namespace recompiler::fe::arm64_v8a {
    void B_IR(ir::Ir &ir, const uint32_t inst) {
        ir.type = ir::IrOperationType::Ir_BRANCH_OP;

        if (const uint32_t imm26 = inst & 0x03FFFFFF; imm26&0x02000000)
            ir.offset|=static_cast<int64_t>(imm26 | 0xFC000000);
        else ir.offset|=imm26;

        ir.offset *= 4;
    }
}