#include "fe/frontend_processor_caps.h"
#include "be/backend_processor_caps.h"
#include "block_linking.h"


namespace recompiler {

BlockLinking::BlockLinking(void *l_class, const c_function_stub_t c_function_for_fallback) : layer(l_class), c_stub_function(c_function_for_fallback) {}


void BlockLinking::pc_set_block(const std::uint64_t pc, void *block) {
    pc_x_block[pc].addr = block;
}

void BlockLinking::pc_set_entry(const std::uint64_t pc) {
    pc_entry_points[pc] = &pc_x_block[pc];
}

std::pair<void *,uint64_t> BlockLinking::get_cfg_jump_addr(const ir::Ir &ir) {
    if (!ir_is_a_branch(ir))
        return {};
    return [&] {
        const auto pc_val = ir.pc + ir.ir_r_list.begin()->value;
        if (!pc_x_block.contains(pc_val)) {
            c_stub_function(layer, pc_val);
        }
        pc_x_block[pc_val].link++;
        return std::make_pair(pc_x_block[pc_val].addr, pc_val);
    }();
}

bool BlockLinking::is_entry_point(const uint64_t pc) {
    return std::ranges::find(pc_entries, pc) != pc_entries.end();
}

void BlockLinking::remove_unreachable_cfgs(const std::unique_ptr<be::BackendProcessorCaps> &hostpr) {
    for (auto pc_x_blk_it = hostpr->pc_x_compiled_block.begin();
            pc_x_blk_it != hostpr->pc_x_compiled_block.end(); ) {

        const auto pc_key = pc_x_blk_it->first;
        if (pc_entry_points.contains(pc_key)) {
            ++pc_x_blk_it;
            continue;
        }
        if (pc_x_block[pc_key].link==0) {
            pc_x_blk_it = hostpr->pc_x_compiled_block.erase(pc_x_blk_it);
        }

    }
}

bool BlockLinking::set_first_block(const void *begin, const size_t sz) {
    constexpr uint8_t epilogue_signature[] = {0xC9, 0xC3};
    if (!leave_addr)
        return (leave_addr = memmem(begin, sz, epilogue_signature, sizeof(epilogue_signature)));
    return false;
}

void BlockLinking::verify_or_populate_runtime_jmp_table(fe::HashableLinkAddr *table) {
    if (!jump_table_ok) {
        for (size_t i=0;i<0x400;i++)
            if (table[i].jump_addr==nullptr)
                table[i].jump_addr=leave_addr;
    }
    jump_table_ok=true;
}
}
