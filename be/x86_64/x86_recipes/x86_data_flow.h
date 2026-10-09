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


    static void MOV_R64_MEM_OR_MEM_R64(CompiledStream &cs, X86RegisterType dest, X86RegisterType base, const int32_t offset, const bool m64_to_r64) {
        uint8_t rex = 0x48;
        if (static_cast<uint32_t>(dest) >= 8) rex |= 0x04;
        if (static_cast<uint32_t>(base) >= 8) rex |= 0x01;

        cs.write_bytes({rex, static_cast<uint8_t>(m64_to_r64?0x8B:0x89)});  // MOV r64, r/m64 : // MOV r/m64, r64

        if (offset >= -128 && offset <= 127) {
            // mod=01, 8-bit displacement
            cs.write_bytes({static_cast<uint8_t>(0x40 | ((static_cast<uint32_t>(dest) & 7) << 3) | (static_cast<uint32_t>(base) & 7)), static_cast<uint8_t>(offset)});
        } else {
            // mod=10, 32-bit displacement
            cs.write_into(0x80 | ((static_cast<uint32_t>(dest) & 7) << 3) | (static_cast<uint32_t>(base) & 7));
            cs.write_into<uint32_t>(offset);
        }
    }

    static void MOV_R64_MEM(CompiledStream &cs, const X86RegisterType dest, const X86RegisterType base, const int32_t offset) {
        MOV_R64_MEM_OR_MEM_R64(cs, dest, base, offset, true);
    }

    static void MOV_MEM_R64(CompiledStream &cs, const X86RegisterType dest, const X86RegisterType base, const int32_t offset) {
        MOV_R64_MEM_OR_MEM_R64(cs, dest, base, offset, false);
    }


    static void MOV_R64_IMM64(CompiledStream &cs, X86RegisterType dest, const uint64_t value) {
        // REX.B extends the register number for R8-R15
        cs.write_into(static_cast<uint8_t>(0x48 | (static_cast<uint8_t>(dest) >= 8 ? 0x01 : 0x00)));

        // MOV r64, imm64
        cs.write_into(static_cast<uint8_t>(0xB8 + (static_cast<uint8_t>(dest) & 7)));
        // imm64, little-endian
        cs.write_into<uint64_t>(value);
    }
};
}
