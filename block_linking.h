#pragma once
#include "ir/types.h"
#include <unordered_map>
#include <cstdint>

namespace recompiler {
struct LinkInfo {
    void *addr;
    size_t link; // count of link against this addr
};
namespace be { class BackendProcessorCaps; }


typedef void * (*c_function_stub_t)(void *, uint64_t);

class BlockLinking {
public:
    BlockLinking(void *l_class, c_function_stub_t c_function_for_fallback);
    void pc_set_block(std::uint64_t pc, void *block);
    void pc_set_entry(std::uint64_t pc);
    std::pair<void *,uint64_t> get_cfg_jump_addr(const ir::Ir &ir);


    void remove_unreachable_cfgs(const std::unique_ptr<be::BackendProcessorCaps> &hostpr);

    void *layer;
    c_function_stub_t c_stub_function;
    std::unordered_map<uint64_t, LinkInfo> pc_x_block;
    std::unordered_map<uint64_t, LinkInfo*> pc_entry_points; // exclude from dead-code optimizations

};
}
