#include "types.h"
#include "asm_isi/arm_v8a_all.h"

namespace recompiler::ir {
std::string to_string(const IrOperationType type) {
    switch (type) {
        case IrOperationType::Ir_NOP_OP:
            return "NOP (does nothing, just code alignment)";
        case IrOperationType::Ir_UJUMP_OP:
            return "UJUMP (unconditional jump) to a r/m";
        case IrOperationType::Ir_RET_OP:
            return "RET (ends of jit code)";
        case IrOperationType::Ir_STORE_OP:
            return "STORE (stores to a r/m)";
        case IrOperationType::Ir_LOAD_OP:
            return "LOAD (loads to a r/m)";
        case IrOperationType::Ir_SHR_OP:
            return "SHR (shift right)";
        case IrOperationType::Ir_AND_OP:
            return "AND (bitwise and)";
        case IrOperationType::Ir_ADD_OP:
            return "ADD (adds)";
        case IrOperationType::Ir_MOVE_OP:
            return "MOVE (moves from r/m)";
        case IrOperationType::Ir_CMP_OP:
            return "CMP (cmp a value in r)";
        case IrOperationType::Ir_ADROF_OP:
            return "ADROF (get addr of r+imm)";
        case IrOperationType::Ir_MUL_OP:
            return "MUL (multiplies)";

            default:
            return "NONE (?, inv ir type)";
    }
}

std::optional<Ir> default_arm64_to_ir(const uint32_t inst) {
    if (asm_isi::ArmV8A_Math::__NOP() == inst)
        return Ir{IrOperationType::Ir_NOP_OP};
#if 0
    if (asm_isi::ArmV8A_Functions::__RET() == inst)
        return Ir{IrOperationType::Ir_RET_OP};
#endif
    return {};
}
}
