#pragma once
#include "../backend_processor_caps.h"

namespace recompiler::be::x86_64 {
class X86InplaceAsm final : public BackendProcessorCaps {
public:
    X86InplaceAsm();

    void compile_irs(uint64_t pc_, const std::vector<ir::Ir> &irs_ops) override;
};
}
