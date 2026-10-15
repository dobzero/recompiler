#include "processor_arm64_v8a.h"

#include <algorithm>

recompiler::front::arm64_v8a::ProcessorArm64v8a::ProcessorArm64v8a(LayerState * layer_state) : FrontendProcessorCaps(layer_state, sf_::Frontend_ARM_V8_A) {
    add_supported_backend_targets({
        SupportedBackends::Arch_X_86_64}
        );
    a_r_m_ctx.layer_addr = layer_state;
}

bool recompiler::front::arm64_v8a::ProcessorArm64v8a::is_pc_compiled(const uint64_t pc) {
    if (cached_pc_region.contains(pc))
        return true;

    return false;
}

namespace recompiler {
using namespace front::arm64_v8a;
using I_M = InstructionMeta;
const std::vector arm64_list_default_table={
    I_M{.mask = 0xFC000000, .value = 0x14000000, .conv_func_ir_callback = a_b_ir, .hits = 0},
    I_M{.mask = 0xD65F03C0, .value = 0xD65F03C0, .conv_func_ir_callback = a_ret_ir, .hits = 0}
};


inplace_vector<ir::Ir, 15> arm64_to_ir(std::vector<InstructionMeta> &arm64_list_ord, const uint32_t inst, const uint64_t pc) {
    inplace_vector<ir::Ir, 15> ir_r;
    if (const auto default_ir = ir::default_arm64_to_ir(inst)) {
        ir_r.emplace_back(*default_ir);
    } else {
        for (auto arm64_list_it = arm64_list_ord.begin(); arm64_list_it!=arm64_list_ord.end(); ) {
            if ((inst & arm64_list_it->mask) != arm64_list_it->value) {
                ++arm64_list_it;
                continue;
            }
            arm64_list_it->hits++;
            arm64_list_it->conv_func_ir_callback(ir_r, inst, pc);

            break;
        }
    }
    for (auto &ir_op_type : ir_r) {
        ir_op_type.pc = pc;
    }

    return ir_r;
}
}

uint64_t recompiler::front::arm64_v8a::ProcessorArm64v8a::compile_irs_from_pc(const uint64_t pc_target) {
    if (arm64_inst_ordered.empty())
        arm64_inst_ordered=arm64_list_default_table;

    bool cfg_block_finished=false;
    std::array pc_ir_x_pc{pc_target, pc_target};
    do {
        Arm64CachedRegion cached_list = {};
        while (cached_list.last_inst_list.size() != cached_list.last_inst_list.capacity()) {
            if (!layer_state->is_reachable(pc_ir_x_pc[0])) {
                break;
            }
            cached_list.last_inst_list.emplace_back(layer_state->read_u32(pc_ir_x_pc[0]));
            pc_ir_x_pc[0]+=4;
        }

        for (const auto &inst_arm64 : cached_list.last_inst_list) {
            if (inst_arm64) {
                auto ir_value = arm64_to_ir(arm64_inst_ordered, inst_arm64, pc_ir_x_pc[1]);
                pc_ir_x_pc[1]+=4;
                for (const auto &ir_valid : ir_value) {
                    this->ir_statistics[ir_valid.type]++;
                    cached_list.irs_list.emplace_back(ir_valid);
                }

                if (ir_search_for_cfg_end(cached_list.irs_list))
                    if ((cfg_block_finished=true))
                        break;
            } else {
                break;
            }
        }
        std::ranges::sort(arm64_inst_ordered, [](const auto &first, const auto &second) {
            return first.hits > second.hits;
        });

        cached_pc_region.insert_or_assign(pc_target, cached_list);
    } while (!cfg_block_finished);

    return pc_target;
}

uint64_t recompiler::front::arm64_v8a::ProcessorArm64v8a::get_reg(const AliasRegisters &reg) {
    if (reg==AliasRegisters::PC)
        return a_r_m_ctx.pc;
    return -1;
}

std::vector<recompiler::ir::Ir> recompiler::front::arm64_v8a::ProcessorArm64v8a::get_irs_from_pc(const uint64_t pc) {
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
