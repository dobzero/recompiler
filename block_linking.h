#pragma once
#include <cstdint>
#include <unordered_map>

#include "ir/types.h"

namespace recompiler {

struct LinkInfo {
    void *addr;
    size_t link; // count of link against this addr
};


typedef void * (*c_function_stub_t)(void *, uint64_t);

class BlockLinking {
public:
    BlockLinking(void *l_class, c_function_stub_t c_function_for_fallback);
    void pc_set_block(std::uint64_t pc, void *block);
    void * fix_branch_ir(const ir::Ir &ir, uint64_t last_pc);

    void *layer;
    c_function_stub_t c_stub_function;
    std::unordered_map<uint64_t, LinkInfo> pc_x_block;

};
}
