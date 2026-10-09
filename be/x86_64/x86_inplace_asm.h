#pragma once
#include "../backend_processor_caps.h"
#include "be/compiled_stream.h"

namespace recompiler::be::x86_64 {
class X86InplaceAsm final : public BackendProcessorCaps {
public:
    X86InplaceAsm();

    void compile_irs(uint64_t pc_, const std::vector<ir::Ir> &irs_ops, BlockLinking &linker, SupportedFrontends fe_type) override;
    size_t execute_at_pc(uint64_t pc_, void *thr_addr) override;

    std::unordered_map<uint64_t, CompiledStream> compiled_pcs;
};
}
