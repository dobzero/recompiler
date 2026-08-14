#pragma once
#include <optional>

#include "platform_features.h"

namespace recompiler::ir {
enum class IrOperationType {
    Ir_NOP_OP
};
using ir_op=IrOperationType;

class Ir {
public:
    explicit Ir(const IrOperationType _type) : type(_type) {}

    // we can't mimify theses instructions with specific arch dependencies
    std::optional<ProcessorArchType> is_exclusive_of_arch_type;
    IrOperationType type;
};

}
