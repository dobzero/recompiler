#pragma once
#include "block_linking.h"
#include "x86_functions.h"

namespace recompiler::be::x86_64::x86_recipes {
struct All {
    static void NOP(CompiledStream &cs) {
        cs.write_into<uint8_t>(0x90);
    }

    static void JMP_ABS(CompiledStream & cs, const void * addr) {
        cs.write_bytes({0x48, 0xB8});
        cs.write_into<uint64_t>(reinterpret_cast<uint64_t>(addr));

        // jmp rax
        cs.write_bytes({0xFF, 0xE0});
    }
    static void JMP_REG(CompiledStream& cs, X86RegisterType reg) {
        const auto r = static_cast<uint8_t>(reg);
        // REX.B extends the r/m register field for R8-R15.
        const uint8_t rex = (r >= 8) ? 0x41 : 0x40;
        if (r >= 8) {
            cs.write_bytes({rex});
        }
        cs.write_bytes({0xFF, static_cast<uint8_t>(0xE0 | (r & 7))});
    }

    static void SHR(CompiledStream &cs, const X86RegisterType reg, uint8_t shift) {
        uint8_t rex = 0x48; // REX.W

        uint8_t modrm = 0xC0 | (5 << 3) | (static_cast<uint8_t>(reg) & 7);
        cs.write_bytes({rex, 0xC1, modrm, shift});
    }

    static void LEA_R64(CompiledStream& cs, X86RegisterType dest, X86RegisterType base, const int32_t offset) {
        const auto d = static_cast<uint8_t>(dest);
        const auto b = static_cast<uint8_t>(base);

        uint8_t rex = 0x48; // REX.W
        if (d >= 8)
            rex |= 0x04; // REX.R
        if (b >= 8)
            rex |= 0x01; // REX.B
        cs.write_bytes({rex, 0x8D});
        // RBP/R13 cannot use mod=00 with r/m=101
        // to represent [base] without displacement.
        const bool special_zero = (offset == 0 && (b & 7) == 5);
        uint8_t mod;
        if (offset == 0 && !special_zero)
            mod = 0;
        else if (offset >= -128 && offset <= 127)
            mod = 1;
        else
            mod = 2;
        const bool needs_sib = (b & 7) == 4;
        const uint8_t modrm =(mod << 6) |((d & 7) << 3) |(needs_sib ? 4 : (b & 7));
        cs.write_bytes({modrm});
        if (needs_sib)
            cs.write_bytes({static_cast<uint8_t>(0x20 | (b & 7))});
        if (mod == 1)
            cs.write_bytes({static_cast<uint8_t>(offset)});
        else if (mod == 2)
            cs.write_into<int32_t>(offset);
    }
};
}