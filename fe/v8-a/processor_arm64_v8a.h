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

template<typename T, size_t N>
class NonReallocatableVector {
public:
    T & push_back(const T value) {
        return data[i_size++]=value;
    }
    auto size() const {
        return i_size;
    }
    auto capacity() const {
        return data.size();
    }
    auto empty() const {
        return size()==0;
    }

    operator std::span<T>() {
        return std::span(&data[0], i_size);
    }
    auto & operator [](const size_t i) const {
        return data[i];
    }
    auto begin() const {
        return data.begin();
    }
    auto end() const {
        return data.begin()+i_size;
    }
private:
    size_t i_size=0;
    std::array<T, N> data;
};

struct Arm64CachedRegion {
    // todo: this cached irs lists should be bigger
    NonReallocatableVector<uint32_t, 60> last_inst_list{}; // 0 isn't a valid arm64 instruction, so, it's our end mark
    NonReallocatableVector<ir::Ir, 60> irs_list{};
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
