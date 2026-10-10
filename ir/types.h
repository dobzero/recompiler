#pragma once
#include <optional>

#include "platform_features.h"
#include "inplace_vector.h"

namespace recompiler::ir {
enum class IrOperationType {
    Ir_Default, // just for construction got esy
    Ir_NOP_OP,
    Ir_BRANCH_OP,
    Ir_RET_OP,

    Ir_STORE, // has 2 or more arguments (1:offset,2:immediate/faster), if a 3, third ever is a faster type
};
using ir_op=IrOperationType;

class IrOperand {
public:
    enum class Faster {
        General, // any gpr of computer
        Thread=0x10000000 // thr, the native guest thread context
    };
    enum class Type {
        Immediate,
        Offset, // also immediate, but our ir type is a branch (Ir_BRANCH_OP, Ir_RET_OP) or Ir_STORE
        Faster
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
    IrOperationType type{IrOperationType::Ir_Default};
    inplace_vector<IrOperand, 3> ir_r_list;
    uint64_t pc{};
};


std::optional<Ir> default_arm64_to_ir(uint32_t inst);
bool ir_search_for_cfg_end(const std::span<const Ir> &irs)  ;
bool ir_is_a_branch(const Ir &ir);


constexpr auto fthread = static_cast<uint64_t>(IrOperand::Faster::Thread);
inline void ir_thr_set(Ir &ir, uint64_t offset, uint64_t value) {
    using Ir_Type = IrOperand::Type;
    ir.type=IrOperationType::Ir_STORE;
    ir.ir_r_list.emplace_back(Ir_Type::Offset, offset);
    ir.ir_r_list.emplace_back(Ir_Type::Immediate, value);
    ir.ir_r_list.emplace_back(Ir_Type::Faster, fthread);

}
inline void ir_thr_get(Ir &ir, uint8_t dest, uint64_t offset) {
    using Ir_Type = IrOperand::Type;
    ir.type=IrOperationType::Ir_STORE;

    ir.ir_r_list.emplace_back(Ir_Type::Offset, offset);
    ir.ir_r_list.emplace_back(Ir_Type::Faster, dest);
    ir.ir_r_list.emplace_back(Ir_Type::Faster, fthread);
}

}
