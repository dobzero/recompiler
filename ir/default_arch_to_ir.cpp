#include "types.h"
#include "asm_isi/arm_v8_a.h"

namespace recompiler::ir {
std::optional<Ir> default_arm64_to_ir(const uint32_t inst) {
    if (asm_isi::ArmV8_A::__NOP() == inst)
        return Ir{IrOperationType::Ir_NOP_OP};

    return {};
}
}
