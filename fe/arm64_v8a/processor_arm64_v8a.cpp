#include "processor_arm64_v8a.h"

#include <algorithm>

recompiler::fe::arm64_v8a::ProcessorArm64v8a::ProcessorArm64v8a(LayerState * layer_state) : FrontendProcessorCaps(layer_state, sf_::Frontend_ARM_V8_A) {
    add_supported_backend_targets({
        SupportedBackends::Arch_X_86_64}
        );
}

bool recompiler::fe::arm64_v8a::ProcessorArm64v8a::is_pc_compiled(const uint64_t pc) {
    if (cached_pc_region.contains(pc))
        return true;

    return false;
}

namespace recompiler {
using namespace fe::arm64_v8a;
using I_M = InstructionMasks;
const std::vector arm64_list_default_table={
    I_M{.mask = 0xFC000000, .value = 0x14000000, .or_conv_func_ir = B_IR, .hits = 0}
};


ir::Ir conv_arm64_to_ir(std::vector<InstructionMasks> &arm64_list_ord, const uint32_t inst) {
    if (const auto default_ir = ir::default_arm64_to_ir(inst)) {
        return *default_ir;
    }
    ir::Ir ir_r{};
    for (auto arm64_list_it = arm64_list_ord.begin(); arm64_list_it!=arm64_list_ord.end(); ) {
        if ((inst & arm64_list_it->mask) != arm64_list_it->value) {
            ++arm64_list_it;
            continue;
        }
        arm64_list_it->hits++;
        arm64_list_it->or_conv_func_ir(ir_r, inst);

        break;
    }

    return ir_r;
}
}

uint64_t recompiler::fe::arm64_v8a::ProcessorArm64v8a::compile_irs_from_pc(const uint64_t pc_target) {
    Arm64CachedRegion cached_list;
    if (arm64_inst_ordered.empty())
        arm64_inst_ordered=arm64_list_default_table;

    do {
        cached_list={};
        uint64_t first_pc = pc_target;
        uint64_t ir_x_pc = first_pc;
        while (cached_list.last_inst_list.size() != cached_list.last_inst_list.capacity()) {
            if (!layer_state->is_reachable(first_pc)) {
                break;
            }
            cached_list.last_inst_list.emplace_back(layer_state->read_u32(first_pc));
            first_pc+=4;
        }

        for (const auto &inst_arm64 : cached_list.last_inst_list) {
            if (inst_arm64) {
                auto ir_value = conv_arm64_to_ir(arm64_inst_ordered, inst_arm64);
                ir_value.pc=ir_x_pc;
                ir_x_pc += 4;
                cached_list.irs_list.emplace_back(ir_value);
            } else {
                break;
            }
        }
        std::ranges::sort(arm64_inst_ordered, [](const auto &first, const auto &second) {
            return first.hits > second.hits;
        });
        cached_list.irs_list.emplace_back(ir::IrOperationType::Ir_Default);

        cached_pc_region.insert_or_assign(pc_target, cached_list);
    } while (!ir_search_for_cfg_end(cached_list.irs_list));

    return pc_target;
}

uint64_t recompiler::fe::arm64_v8a::ProcessorArm64v8a::get_reg(const AliasRegisters &reg) {
    if (reg==AliasRegisters::PC)
        return arm_v8a_state.pc;
    __builtin_unreachable();
}

std::vector<recompiler::ir::Ir> recompiler::fe::arm64_v8a::ProcessorArm64v8a::get_irs_from_pc(const uint64_t pc) {
    const auto &[last_inst_list, irs_list] = cached_pc_region.at(pc);
    if (irs_list.empty()) {
        return {};
    }
    std::vector<ir::Ir> ir_list;
    for (size_t i=0;i<irs_list.size();i++) {

        ir_list.push_back(irs_list[i]);
    }
    return ir_list;
}
