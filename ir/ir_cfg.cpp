#include "types.h"

namespace recompiler::ir {
bool ir_search_for_cfg_end(const std::span<Ir> &irs) {
    for (const auto &ir : irs) {
        if (ir_is_a_branch(ir))
            return true;
    }
    return false;
}

bool ir_is_a_branch(const Ir &ir) {
    if (ir.type==IrOperationType::Ir_Default)
        return true; // todo not right, but useful by now
    return false;
}
}
