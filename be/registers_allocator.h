#pragma once
#include <cstdint>
#include <optional>
#include <unordered_map>

namespace recompiler::be {
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
    const RegisterRuntimeMeta & get_allocated(const uint64_t target) {
        return indexable_used_regs[target];
    }


    uint64_t default_width=0;

    std::optional<const RegisterRuntimeMeta *> default_rt_thr_reg; // register used to save guest thread context
    std::unordered_map<uint32_t, RegisterRuntimeMeta> indexable_used_regs;
};
}
