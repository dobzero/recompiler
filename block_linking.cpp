#include "block_linking.h"
namespace recompiler {

BlockLinking::BlockLinking(void *l_class, const c_function_stub_t c_function_for_fallback) : layer(l_class), c_stub_function(c_function_for_fallback) {}


void BlockLinking::pc_set_block(const std::uint64_t pc, void *block) {
    pc_x_block[pc].addr = block;
}

void * BlockLinking::get_jump_addr_from_ir_x_pc(const ir::Ir &ir, const uint64_t last_pc) {
    if (!ir_is_a_branch(ir))
        return nullptr;

    return [&] {
        if (pc_x_block.contains(last_pc)) {
            auto &[addrblk, sizeblk] = pc_x_block[last_pc];
            sizeblk++;
            return addrblk;
        }
        return c_stub_function(layer, last_pc);
    }();
}
}
