#include "registers_allocator.h"
namespace recompiler::back {
void RegistersAllocator::allocate_for_thr(const uint64_t index) {

    if (default_rt_thr_reg)
        default_rt_thr_reg.reset();
    allocate(index, index);
    auto & thr_reg = get_allocated(index);
    thr_reg.is_reusable=false;
    default_rt_thr_reg.emplace(thr_reg);
}

void RegistersAllocator::allocate(const uint64_t target, const uint64_t index) {
    if (const auto indexable_it = indexable_in_use_r_list.find(index); indexable_it != indexable_in_use_r_list.end()) {
        if (auto &register_meta = indexable_it->second; register_meta.is_reusable) {
            flush_registers.emplace_back(register_meta);
            register_meta.target = target;
            register_meta.index = index;
        }
    } else if (target == index) {
        if (!indexable_in_use_r_list.contains(index)) {
            indexable_in_use_r_list.emplace(index, RegisterRuntimeMeta{
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
