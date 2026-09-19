#pragma once
#include <optional>

#include "platform_features.h"

namespace recompiler::ir {
enum class IrOperationType {
    Ir_Default, // just for construction got esy
    Ir_NOP_OP
};
using ir_op=IrOperationType;

class Ir {
public:
    Ir()=default;
    explicit Ir(const IrOperationType _type) : type(_type) {}

    // we can't mimify theses instructions with specific arch dependencies
    std::optional<ProcessorArchType> is_exclusive_of_arch_type;
    IrOperationType type{IrOperationType::Ir_Default};
};


std::optional<Ir> default_arm64_to_ir(uint32_t inst);
bool ir_search_for_cfg_end(const std::span<Ir> &irs)  ;

}
