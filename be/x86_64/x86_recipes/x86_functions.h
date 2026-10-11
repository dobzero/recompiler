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
        RET(cs);
    }

    static void do_prologue(CompiledStream & cs, const X86RegistersAllocator & alloc) {
        DataFlow::MOV_R64_R64(cs, alloc.get_thr_reg(), X86RegisterType::RDI);
    }
};
}
