#pragma once

namespace recompiler {
enum class SupportedFrontends {
    Frontend_ARM_946E_S, // DS
    Frontend_ARM_7TDMI, // DS
    Frontend_ARM_V8_A, // Switch
    Frontend_ARM_32, // Switch
    Frontend_MIPS_I, // PS1 processor
    Frontend_MIPS_IV, // PS2 processor (with s0n1 extensions)
};
using sf_= SupportedFrontends;
enum class SupportedBackends {
    Arch_X_86_64, Arch_ARM_64
};
}
