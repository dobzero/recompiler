#pragma once

#include "../x86_registers_allocator.h"
#include "back/compiled_stream.h"
#include <cstdint>

namespace recompiler::back::x86_64::x86_recipes {
struct DataFlow {
    static void PUSH(CompiledStream &cs, const X86RegisterType r__) {
        if (r__ >= X86RegisterType::R8) {
            cs.write_bytes({0x41, /* REX.B */ static_cast<uint8_t>(0x50 + (static_cast<uint8_t>(r__) - 8))});
        } else {
            cs.write_into(static_cast<uint8_t>(0x50 + (static_cast<uint8_t>(r__))));
        }
    }
    static void MOV_R64_R64(CompiledStream &cs, X86RegisterType dst, X86RegisterType src) {
        uint8_t rex = 0x48; // REX.W
        if (static_cast<uint8_t>(src) >= 8)
            rex |= 0x04;    // REX.R
        if (static_cast<uint8_t>(dst) >= 8)
            rex |= 0x01;    // REX.B

        cs.write_bytes({rex, 0x89, static_cast<uint8_t>(0xC0 | ((static_cast<uint8_t>(src) & 7) << 3) | (static_cast<uint8_t>(dst) & 7))});
    }


    static void mov_r64m_or_r64m(CompiledStream& cs, X86RegisterType reg, X86RegisterType base, const int32_t offset, const bool to_register) {
        const auto r = static_cast<uint8_t>(reg);
        const auto b = static_cast<uint8_t>(base);

        uint8_t rex = 0x48; // REX.W
        if (r >= 8)
            rex |= 0x04; // REX.R
        if (b >= 8)
            rex |= 0x01; // REX.B
        cs.write_bytes({rex, static_cast<uint8_t>(to_register ? 0x8B : 0x89)});
        // Use disp8 whenever possible.
        const uint8_t mod = (offset >= -128 && offset <= 127) ? 0x40 : 0x80;
        const bool needs_sib = (b & 7) == 4;
        const uint8_t modrm =
            mod | ((r & 7) << 3) |
            (needs_sib ? 4 : (b & 7));
        cs.write_bytes({modrm});
        if (needs_sib) {
            // Scale=1, no index, base=RSP/R12.
            cs.write_bytes({static_cast<uint8_t>(0x20 | (b & 7))});
        }
        if (mod == 0x40) {
            cs.write_bytes({static_cast<uint8_t>(offset)});
        } else {
            cs.write_into<int32_t>(offset);
        }
    }

    static void MOV_R64_MEM(CompiledStream &cs, const X86RegisterType dest, const X86RegisterType base, const int32_t offset) {
        mov_r64m_or_r64m(cs, dest, base, offset, true);
    }

    static void MOV_MEM_R64(CompiledStream &cs, const X86RegisterType src, const X86RegisterType base, const int32_t offset) {
        mov_r64m_or_r64m(cs, src, base, offset, false);
    }


    static void MOV_R64_IMM64(CompiledStream &cs, X86RegisterType dest, const int64_t value) {
        // REX.B extends the register number for R8-R15
        cs.write_into(static_cast<uint8_t>(0x48 | (static_cast<uint8_t>(dest) >= 8 ? 0x01 : 0x00)));

        // MOV r64, imm64
        cs.write_into(static_cast<uint8_t>(0xB8 + (static_cast<uint8_t>(dest) & 7)));
        // imm64, little-endian
        cs.write_into<int64_t>(value);
    }
};
}
