#pragma once
#include "be/registers_allocator.h"

namespace recompiler::be::x86_64 {
enum class X86RegisterType {
    RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15
};
class X86RegistersAllocator : public RegistersAllocator {
    public:
    explicit X86RegistersAllocator(X86RegisterType thr_goes_at);

    X86RegisterType get_thr_reg() const;
};
}
