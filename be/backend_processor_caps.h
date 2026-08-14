#pragma once
#include "types.h"
#include "ir/types.h"
#include <unordered_map>
#include <cstdint>
#include <vector>

namespace recompiler::be {
struct CompiledBlockDetails {
    // block location and size
    void *begin;
    size_t size;


};

class BackendProcessorCaps {
    public:
    virtual ~BackendProcessorCaps() = default;

    explicit BackendProcessorCaps(const SupportedBackends _type) : type(_type) {}
    virtual void compile_irs(uint64_t pc_, const std::vector<ir::Ir> &irs_ops)=0;
    SupportedBackends type;

    std::unordered_map<uint64_t, CompiledBlockDetails> pc_x_compiled_block; // also used in mips and 32 bits architectures
};
}
