#pragma once
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "fe/frontend_processor_caps.h"
#include "ir/types.h"

namespace recompiler::fe::v8_a {
struct HashableLinkAddr {
    uint64_t target_pc;
    void * jump_addr;
};

struct Arm64ProcessorState {
    std::array<uint64_t, 31> gpr_list_;

    uint64_t sp, pc;
    uint64_t p_state;

    std::array<HashableLinkAddr, 0x3FF> fastjump_table; // used when a jump with a register appears while runtime
};

struct Arm64CachedRegion {
    inplace_vector<uint32_t, 60> last_inst_list{}; // 0 isn't a valid arm64 instruction, so, it's our end mark
    inplace_vector<ir::Ir, 60> irs_list{};
};

class ProcessorArm64v8a final : public FrontendProcessorCaps {
public:
    explicit ProcessorArm64v8a(LayerState* layer_state);

    bool is_pc_compiled() override;
    void * get_thr_addr() override {
        return &arm_v8a_state;
    }
    uint64_t compile_irs_from_pc() override;
    uint64_t get_reg(const AliasRegisters &reg) override;
    std::vector<ir::Ir> get_irs_from_pc(uint64_t pc) override;


    std::unordered_map<uint64_t, Arm64CachedRegion> cached_pc_region;
    Arm64ProcessorState arm_v8a_state={};
};
}
