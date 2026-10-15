#pragma once
#include <unordered_map>
#include <optional>
#include <cstdint>
#include <vector>

namespace recompiler::back {
struct RegisterRuntimeMeta {
    uint64_t index, target; // maps host index to target guest register
    uint64_t width;

    uint32_t is_dirty: 1;
    uint32_t is_reusable: 1;
};
class RegistersAllocator {
    public:
    explicit RegistersAllocator(const uint64_t width) : default_width(width) {}
    void allocate_for_thr(uint64_t index);
    void allocate(uint64_t target, uint64_t index);
    auto & get_allocated(const uint64_t target) {
        return indexable_in_use_r_list[target];
    }


    uint64_t default_width=0;

    std::optional<const RegisterRuntimeMeta> default_rt_thr_reg; // register used to save guest thr_ctx
    std::vector<RegisterRuntimeMeta> flush_registers; // registers waiting to be flushed to guest thr_ctx
    std::unordered_map<uint32_t, RegisterRuntimeMeta> indexable_in_use_r_list;
};
}
