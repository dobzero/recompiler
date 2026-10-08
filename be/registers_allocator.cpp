#include "registers_allocator.h"
namespace recompiler::be {
void RegistersAllocator::allocate_for_thr(const uint64_t index) {

    if (default_rt_thr_reg)
        default_rt_thr_reg.reset();
    allocate(index, index);
    default_rt_thr_reg.emplace(&get_allocated(index));
}

void RegistersAllocator::allocate(uint64_t target, const uint64_t index) {
    if (const auto indexable_it = indexable_used_regs.find(index); indexable_it != indexable_used_regs.end()) {
        if (auto &register_meta = indexable_it->second; register_meta.is_reusable) {
            register_meta.target = target;
        }
    } else if (target == index) {
        if (!indexable_used_regs.contains(target)) {
            indexable_used_regs.emplace(target, RegisterRuntimeMeta{
                .index = index,
                .target = target,
                .width = default_width,
                .is_dirty = false,
                .is_reusable = true
            });
        }
    }

}
}
