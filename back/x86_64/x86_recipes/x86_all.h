#pragma once
#include "block_linking.h"
#include "x86_functions.h"

namespace recompiler::back::x86_64::x86_recipes {
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


    static void LEA_R64_IMM(CompiledStream &cs, X86RegisterType dst, X86RegisterType base, X86RegisterType index, const uint8_t scale, const int32_t displacement) {
        const auto d = static_cast<uint8_t>(dst);
        const auto b = static_cast<uint8_t>(base);
        const auto i = static_cast<uint8_t>(index);

        uint8_t ss;
        switch (scale) {
            case 1: ss = 0; break;
            case 2: ss = 1; break;
            case 4: ss = 2; break;
            case 8: ss = 3; break;
            default:
                throw std::invalid_argument{"invalid LEA scale"};
        }
        uint8_t rex = 0x48;
        if (d >= 8) rex |= 0x04;
        if (i >= 8) rex |= 0x02;
        if (b >= 8) rex |= 0x01;
        cs.write_bytes({rex, 0x8D});
        // A SIB byte is required for this addressing form.
        // Use disp32 when nonzero; disp8 can be optimized separately.
        const uint8_t mod = displacement == 0 && (b & 7) != 5 ? 0 : (displacement >= -128 && displacement <= 127) ? 1 : 2;
        const uint8_t modrm = static_cast<uint8_t>((mod << 6) | ((d & 7) << 3) | 0x04);

        const uint8_t sib = static_cast<uint8_t>((ss << 6) | ((i & 7) << 3) | (b & 7));
        cs.write_bytes({modrm, sib});

        if (mod == 1)
            cs.write_bytes({static_cast<uint8_t>(displacement)});
        else if (mod == 2)
            cs.write_into<int32_t>(displacement);
    }



    static void AND_R64_IMM(CompiledStream & cs, X86RegisterType dst, const int32_t imm){
        const auto d = static_cast<uint8_t>(dst);
        // REX.W selects 64-bit operands.
        // REX.B extends the destination register.
        uint8_t rex = 0x48;
        if (d >= 8)
            rex |= 0x01;
        cs.write_bytes({rex});
        if (imm >= -128 && imm <= 127) {
            // AND r/m64, imm8: 83 /4 ib
            cs.write_bytes({0x83, static_cast<uint8_t>(0xE0 | (d & 7)), static_cast<uint8_t>(imm)});
        } else {
            // AND r/m64, imm32: 81 /4 id
            cs.write_bytes({0x81, static_cast<uint8_t>(0xE0 | (d & 7))});
            cs.write_into<int32_t>(imm);
        }
    }

    static void ADD_R64_IMM(CompiledStream & cs, X86RegisterType dst, const int32_t imm) {
        const auto d = static_cast<uint8_t>(dst);
        // REX.W: 64-bit operand
        // REX.B: extended destination register (R8-R15)
        uint8_t rex = 0x48;
        if (d >= 8)
            rex |= 0x01;
        cs.write_bytes({rex});
        if (imm >= -128 && imm <= 127) {
            // ADD r/m64, imm8: 83 /0 ib
            cs.write_bytes({0x83, static_cast<uint8_t>(0xC0 | (d & 7)), static_cast<uint8_t>(imm)});
        } else {
            // ADD r/m64, imm32: 81 /0 id
            cs.write_bytes({0x81, static_cast<uint8_t>(0xC0 | (d & 7))});
            cs.write_into<int32_t>(imm);
        }
    }

    static void SHR_A64_IMM(CompiledStream & cs, X86RegisterType dst, uint8_t shift) {
        const auto d = static_cast<uint8_t>(dst);
        // REX.W: 64-bit operand
        // REX.B: extended destination register
        uint8_t rex = 0x48;
        if (d >= 8)
            rex |= 0x01;
        cs.write_bytes({rex});
        if (shift == 1) {
            // SHR r/m64, 1: D1 /5
            cs.write_bytes({0xD1,static_cast<uint8_t>(0xE8 | (d & 7))});
        } else {
            // SHR r/m64, imm8: C1 /5 ib
            cs.write_bytes({0xC1, static_cast<uint8_t>(0xE8 | (d & 7)), shift});
        }
    }

    static void IMUL_R64_IMM(CompiledStream &cs, X86RegisterType dst, X86RegisterType src, const int32_t imm){
        const auto d = static_cast<uint8_t>(dst);
        const auto s = static_cast<uint8_t>(src);

        // REX.W: 64-bit operand
        // REX.R: extends destination
        // REX.B: extends source
        uint8_t rex = 0x48;
        if (d >= 8)
            rex |= 0x04;
        if (s >= 8)
            rex |= 0x01;
        cs.write_bytes({rex});
        if (imm >= -128 && imm <= 127) {
            // IMUL r64, r/m64, imm8
            cs.write_bytes({0x6B, static_cast<uint8_t>(0xC0 | ((d & 7) << 3) | (s & 7)), static_cast<uint8_t>(imm)});
        } else {
            // IMUL r64, r/m64, imm32
            cs.write_bytes({0x69, static_cast<uint8_t>(0xC0 | ((d & 7) << 3) | (s & 7))});
            cs.write_into<int32_t>(imm);
        }
    }
};
}