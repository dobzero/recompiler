#pragma once
#include <unordered_map>
#include <cstdint>

#include "fe/frontend_processor_caps.h"
#include "ir/types.h"

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

struct Arm64CachedRegion {
    std::array<uint32_t, 30> last_thirty{}; // 0 isn't a valid arm64 instruction, so, it's our end mark
    std::array<ir::Ir, 30> irs_list{};
    size_t ir_count=0;
};

class ProcessorArm64v8a final : public FrontendProcessorCaps {
public:
    explicit ProcessorArm64v8a(LayerState* layer_state);

    bool is_pc_compiled() override;
    bool compile_irs_from_pc() override;

    std::unordered_map<uint64_t, Arm64CachedRegion> cached_pc_region;
    Arm64ProcessorState arm_v8a_state={};
};
}
