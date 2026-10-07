#include "x86_registers_allocator.h"

namespace recompiler::be::x86_64 {
X86RegistersAllocator::X86RegistersAllocator(const X86RegisterType thr_goes_at) : RegistersAllocator(64) {
    allocate_for_thr(static_cast<uint64_t>(thr_goes_at));
}

X86RegisterType X86RegistersAllocator::get_thr_reg() const {
    return static_cast<X86RegisterType>((*default_rt_thr_reg)->index);
}
}
