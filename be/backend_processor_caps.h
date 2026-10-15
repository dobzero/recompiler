#pragma once
#include "block_linking.h"
#include "ir/types.h"
#include "types.h"
#include <unordered_map>
#include <cstdint>
#include <vector>


namespace recompiler::be {
struct CompiledBlockDetails {
    // block location and size
    void *begin;
    size_t size;
    size_t pc_after; // count of instructions
    size_t g_pc_after; // guest pc ends

};
class CompiledStream;
CompiledBlockDetails enable_jit_in(const CompiledStream &compiled, uint64_t final_g_pc);
void disable_jit_in(CompiledBlockDetails &block);

struct ExecutionResult {
    size_t g_pc_begin;
    size_t g_pc_end;
};

class BackendProcessorCaps {
    public:
    virtual ~BackendProcessorCaps() = default;

    explicit BackendProcessorCaps(const SupportedBackends _type) : type(_type) {}
    virtual void compile_irs(uint64_t pc_, const std::vector<ir::Ir> &irs_ops, BlockLinking &linker)=0;
    virtual ExecutionResult execute_at_pc(uint64_t pc_, void * thr_addr)=0;
    SupportedBackends type;

    std::unordered_map<uint64_t, CompiledBlockDetails> pc_x_compiled_block; // also used in mips and 32 bits architectures
};
}
