#pragma once
#include <unordered_map>
#include <cstdint>

#include "fe/frontend_processor_caps.h"
namespace recompiler::fe::v8_a {
struct Arm64ProcessorState {
    // X0–X7: Used to pass arguments into functions and return results.
    // X9–X15: Temporary registers (caller-saved).
    // X19–X28: Callee-saved registers (must be preserved across function calls).
    // X29 (FP): The Frame Pointer
    // X30 (LR): The Link Register
    std::array<uint64_t, 31> gpr_list_; // x0 is always 0;

    uint64_t sp, pc;
    uint64_t p_state;
};

class ProcessorArm64v8a final : public FrontendProcessorCaps {
public:
    ProcessorArm64v8a();

    bool is_pc_compiled() override;

    std::unordered_map<uint64_t, bool> cached_pc_region;
    Arm64ProcessorState arm_v8a_state={};
};
}
