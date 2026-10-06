#pragma once
#include "be/compiled_stream.h"
#include "../backend_processor_caps.h"

namespace recompiler::be::x86_64 {
class X86InplaceAsm final : public BackendProcessorCaps {
public:
    X86InplaceAsm();

    void compile_irs(uint64_t pc_, const std::vector<ir::Ir> &irs_ops) override;
    size_t execute_at_pc(uint64_t pc_) override;
    std::unordered_map<uint64_t, CompiledStream> compiled_pcs;
};
}
