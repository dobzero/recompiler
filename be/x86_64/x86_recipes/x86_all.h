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

    static void SHR(CompiledStream &cs, const X86RegisterType reg, uint8_t shift) {
        uint8_t rex = 0x48; // REX.W

        uint8_t modrm = 0xC0 | (5 << 3) | (static_cast<uint8_t>(reg) & 7);
        cs.write_bytes({rex, 0xC1, modrm, shift});
    }
};
}