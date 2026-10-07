#pragma once

#include "be/x86_64/x86_registers_allocator.h"
#include "../../compiled_stream.h"
#include <cstdint>

namespace recompiler::be::x86_64::x86_recipes {
struct DataFlow {
    static void PUSH(CompiledStream &cs, const X86RegisterType r__) {
        if (r__ >= X86RegisterType::R8) {
            cs.write_bytes({0x41, /* REX.B */ static_cast<uint8_t>(0x50 + (static_cast<uint64_t>(r__) - 8))});
        } else {
            cs.write_into(static_cast<uint8_t>(0x50 + (static_cast<uint64_t>(r__))));
        }
    }
    static void MOV_REG(CompiledStream &cs, X86RegisterType dst, X86RegisterType src) {
        uint8_t rex = 0x48; // REX.W
        if (static_cast<uint64_t>(src) >= 8)
            rex |= 0x04;    // REX.R
        if (static_cast<uint64_t>(dst) >= 8)
            rex |= 0x01;    // REX.B

        cs.write_bytes({rex, 0x89, static_cast<uint8_t>(0xC0 | ((static_cast<uint64_t>(src) & 7) << 3) | (static_cast<uint64_t>(dst) & 7))});
    }
};
}
