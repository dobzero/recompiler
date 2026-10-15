#pragma once
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "front/frontend_processor_caps.h"
#include "arm64_b_list.h"
#include "ir/types.h"

namespace recompiler::front::arm64_v8a {

struct Arm64ProcessorState {
    void * layer_addr;
    std::array<uint64_t, 31> gpr_list_;

    uint64_t sp, pc;
    uint64_t p_state;

    static consteval auto offset_of(const AliasRegisters alias) {
        constexpr auto gpr_offset = offsetof(Arm64ProcessorState, gpr_list_);
        switch (alias) {
            case AliasRegisters::PC:
                return offsetof(Arm64ProcessorState, pc);
            case AliasRegisters::Linker:
                return gpr_offset + sizeof(gpr_list_[0]) * 30;
            case AliasRegisters::HashableTable:
                return offsetof(Arm64ProcessorState, fastjump_table);
        }
        return 0UL;
    }

    std::array<HashableLinkAddr, 0x400> fastjump_table; // used when a jump with a register appears while runtime
};
struct Arm64CachedRegion {
    inplace_vector<uint32_t, 60> last_inst_list{}; // 0 isn't a valid arm64 instruction, so, it's our end mark
    inplace_vector<ir::Ir, 60> irs_list{};
};

class ProcessorArm64v8a final : public FrontendProcessorCaps {
public:
    explicit ProcessorArm64v8a(LayerState* layer_state);

    bool is_pc_compiled(uint64_t) override;
    void * get_thr_addr() override {
        return &a_r_m_ctx;
    }
    uint64_t compile_irs_from_pc(uint64_t) override;
    uint64_t get_reg(const AliasRegisters &reg) override;
    std::vector<ir::Ir> get_irs_from_pc(uint64_t pc) override;
    HashableLinkAddr * get_table() override {
        return a_r_m_ctx.fastjump_table.data();
    }


    std::unordered_map<uint64_t, Arm64CachedRegion> cached_pc_region;
    std::vector<InstructionMeta> arm64_inst_ordered;
    Arm64ProcessorState a_r_m_ctx{};
};
}
