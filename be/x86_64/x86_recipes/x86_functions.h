#pragma once
#include "x86_data_flow.h"

namespace recompiler::be::x86_64::x86_recipes {
struct Functions {

    static void RET(CompiledStream & cs) {
        cs.write_bytes({0xC3});
    }

    static void LEAVE(CompiledStream &cs) {
        cs.write_bytes({0xC9});
    }


    static void do_epilogue(CompiledStream &cs) {
        LEAVE(cs);
        RET(cs);
    }

    static void do_prologue(CompiledStream & cs, const X86RegistersAllocator & alloc) {
        DataFlow::PUSH(cs, X86RegisterType::RBP); // PUSH RBP
        DataFlow::MOV_REG(cs, X86RegisterType::RBP, X86RegisterType::RSP);
        DataFlow::MOV_REG(cs, alloc.get_thr_reg(), X86RegisterType::RDI);
    }
};
}
