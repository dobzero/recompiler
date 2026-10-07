#pragma once
#include "x86_functions.h"

namespace recompiler::be::x86_64::x86_recipes {
struct All {
    static void NOP(CompiledStream &cs) {
        cs.write_into<uint8_t>(0x90);
    }
};
}