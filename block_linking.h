#pragma once
#include "ir/types.h"
#include <unordered_map>
#include <cstdint>
#include <vector>

namespace recompiler {
struct LinkInfo {
    void *addr;
    size_t link; // count of link against this addr
};
namespace be { class BackendProcessorCaps; }
namespace fe { struct HashableLinkAddr; }

typedef void * (*c_function_stub_t)(void *, uint64_t);

class BlockLinking {
public:
    BlockLinking(void *l_class, c_function_stub_t c_function_for_fallback);
    void pc_set_block(std::uint64_t pc, void *block);
    void pc_set_entry(std::uint64_t pc);
    std::pair<void *,uint64_t> get_cfg_jump_addr(const ir::Ir &ir);

    bool is_entry_point(uint64_t pc);
    void remove_unreachable_cfgs(const std::unique_ptr<be::BackendProcessorCaps> &hostpr);

    bool set_first_block(const void * begin, size_t sz);

    void verify_or_populate_runtime_jmp_table(fe::HashableLinkAddr * table);

    void *layer;
    c_function_stub_t c_stub_function;
    void * leave_addr = nullptr;
    std::unordered_map<uint64_t, LinkInfo> pc_x_block;
    std::vector<uint64_t> pc_entries={0};
    std::unordered_map<uint64_t, LinkInfo*> pc_entry_points; // exclude from dead-code optimizations

    bool jump_table_ok=false;
};
}
