
#include "arm64_b_list.h"

#include "processor_arm64_v8a.h"

namespace recompiler::fe::arm64_v8a {
    using A64_S = Arm64ProcessorState;
    void B_IR(inplace_vector<ir::Ir, 10> &ir, const uint32_t inst, const uint64_t pc) {
        uint64_t offset=0;
        if (const uint32_t imm26 = inst & 0x03FFFFFF; imm26&0x02000000)
            offset|=static_cast<int64_t>(imm26 | 0xFC000000);
        else offset|=imm26;

        offset *= 4;
        ir.emplace_n_and_call([&](const std::span<ir::Ir> &ir_v) {
        // save lr to pc + 4 (our ret addr)
            ir_thr_set(ir_v[0], A64_S::offset_of(AliasRegisters::Linker), pc + 4);
        // set pc to offset
            ir_thr_set(ir_v[1], A64_S::offset_of(AliasRegisters::PC), offset);
            ir_thr_get(ir_v[2], 0, A64_S::offset_of(AliasRegisters::PC)); // rax = pc

            // set branch
            ir_v[3].type=ir::IrOperationType::Ir_BRANCH_OP;
            ir_v[3].ir_r_list.emplace_back(ir::IrOperand::Type::Offset, offset);
        }, 4);
    }
}
