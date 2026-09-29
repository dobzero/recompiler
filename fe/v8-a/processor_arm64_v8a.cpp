#include "processor_arm64_v8a.h"

recompiler::fe::v8_a::ProcessorArm64v8a::ProcessorArm64v8a(LayerState* layer_state) : FrontendProcessorCaps(layer_state, sf_::Frontend_ARM_V8_A) {
    add_supported_backend_targets({
        SupportedBackends::Arch_X_86_64}
        );
}

bool recompiler::fe::v8_a::ProcessorArm64v8a::is_pc_compiled() {
    if (cached_pc_region.contains(arm_v8a_state.pc))
        return true;

    return false;
}

namespace recompiler {
ir::Ir conv_arm64_to_ir([[maybe_unused]] const uint32_t inst) {
    if (const auto default_ir = ir::default_arm64_to_ir(inst)) {
        return *default_ir;
    }
    return {};
}
}

uint64_t recompiler::fe::v8_a::ProcessorArm64v8a::compile_irs_from_pc() {
    Arm64CachedRegion cached_list;

    const uint64_t begin_pc_with=arm_v8a_state.pc;
    do {
        uint64_t first_pc = arm_v8a_state.pc;
        for (auto & valid_inst : cached_list.last_thirty) {
            if (!layer_state->is_reachable(arm_v8a_state.pc)) {
                break;
            }
            valid_inst=layer_state->read_u32(arm_v8a_state.pc);
            arm_v8a_state.pc+=4;
        }

        for (const auto &inst_arm64 : cached_list.last_thirty) {
            if (inst_arm64==0)
                break;
            const auto ir_value = conv_arm64_to_ir(inst_arm64);
            cached_list.irs_list[cached_list.ir_count++]=ir_value;
        }
        cached_pc_region.insert_or_assign(first_pc, cached_list);
    } while (!ir_search_for_cfg_end(cached_list.irs_list));

    return begin_pc_with;
}

std::vector<recompiler::ir::Ir> recompiler::fe::v8_a::ProcessorArm64v8a::get_irs_from_pc(const uint64_t pc) {
    const auto &cached_list = cached_pc_region[pc];
    if (cached_list.ir_count==0) {
        return {};
    }
    std::vector<ir::Ir> ir_list;
    for (size_t i=0;i<cached_list.ir_count;i++) {

        ir_list.push_back(cached_list.irs_list[i]);
    }
    return ir_list;
}
