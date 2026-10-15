#pragma once
#include "x86_data_flow.h"

namespace recompiler::back::x86_64::x86_recipes {
struct Functions {

    static void RET(CompiledStream & cs) {
        cs.write_bytes({0xC3});
    }

    static void LEAVE(CompiledStream &cs) {
        cs.write_bytes({0xC9});
    }
    static void CALL(CompiledStream &cs, X86RegisterType reg) {
        const auto r = static_cast<uint8_t>(reg);
        // REX.B extends the r/m field for R8-R15.
        if (r >= 8) {
            cs.write_bytes({0x41});
        }
        // FF /2: CALL r/m64
        // ModR/M: 11 010 r/m
        cs.write_bytes({0xFF, static_cast<uint8_t>(0xD0 | (r & 7))});
    }


    static void do_epilogue(CompiledStream &cs) {
        LEAVE(cs);
        RET(cs);
    }

    static void do_prologue(CompiledStream & cs, const X86RegistersAllocator & alloc) {
        DataFlow::PUSH(cs, X86RegisterType::RBP);
        DataFlow::MOV_R64_R64(cs, X86RegisterType::RBP, X86RegisterType::RSP);
        DataFlow::MOV_R64_R64(cs, alloc.get_thr_reg(), X86RegisterType::RDI);
    }
};
}
