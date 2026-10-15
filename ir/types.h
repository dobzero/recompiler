#pragma once
#include <optional>

#include "platform_features.h"
#include "inplace_vector.h"

namespace recompiler::ir {
enum class IrOperationType {
    Ir_NONE, // just for construction got esy
    Ir_NOP_OP,
    Ir_UJUMP_OP,
    Ir_RET_OP,

    Ir_STORE_OP,
    Ir_LOAD_OP,
    Ir_SHR_OP,
    Ir_AND_OP,
    Ir_ADD_OP,
    Ir_MOVE_OP,
    Ir_CMP_OP,
    Ir_ADROF_OP,
    Ir_MUL_OP
};

std::string to_string(IrOperationType type);
using ir_op=IrOperationType;

class IrOperand {
public:
    enum class Type {
        Immediate,
        Offset, // also immediate, but our ir type is a branch (Ir_BRANCH_OP, Ir_RET_OP) or Ir_STORE
        Register
    };
    IrOperand(const Type _type, const uint64_t val) : type(_type), value(val) {}
    Type type;
    uint64_t value;
};

class Ir {
public:
    Ir()=default;
    explicit Ir(const IrOperationType _type) : type(_type) {}

    // we can't mimify theses instructions with specific arch dependencies
    std::optional<ProcessorArchType> is_exclusive_of_arch_type;
    IrOperationType type{IrOperationType::Ir_NONE};
    inplace_vector<IrOperand, 4> ir_r_list;
    uint64_t pc{};
};


std::optional<Ir> default_arm64_to_ir(uint32_t inst);
bool ir_search_for_cfg_end(const std::span<const Ir> &irs)  ;
bool ir_is_a_branch(const Ir &ir);


using Ir_Type = IrOperand::Type;

}
