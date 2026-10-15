#include "layer.h"

#include <format>
#include <iostream>

#include "front/arm64_v8a/processor_arm64_v8a.h"
#include "back/x86_64/x86_inplace_asm.h"

extern "C" void * layer_stub_function(void * layer_ptr, const uint64_t pc) {
    auto * layer = static_cast<recompiler::Layer *>(layer_ptr);
    void * block_result = layer->compile_cfg_at(pc);
    if (const auto arm64_guest = std::dynamic_pointer_cast<recompiler::front::arm64_v8a::ProcessorArm64v8a>(layer->s_guest_f)) {
        auto &[target_pc, jump_addr] = arm64_guest->a_r_m_ctx.fastjump_table[pc >> 2];
        target_pc = pc;
        jump_addr = block_result;
    }
    return block_result;
}

namespace recompiler {
Layer::Layer() : block_linking(this, layer_stub_function) {
}

// modify what will be the guest and host processor
void Layer::ctrl_modify_archs(const SupportedFrontends guest_type, const SupportedBackends backend_type) {
    if (!s_guest_f || s_guest_f->type!=guest_type) {
        s_guest_f=[&guest_type, this]() -> std::unique_ptr<front::FrontendProcessorCaps> {
            switch (guest_type) {
                case SupportedFrontends::Frontend_ARM_V8_A:
                    return std::make_unique<front::arm64_v8a::ProcessorArm64v8a>(this);
                    break;
                default:
                    return nullptr;
            }
        }();
    }
    if (!s_host_b || s_host_b->type!=backend_type) {
        s_host_b=[&backend_type]() -> std::unique_ptr<back::BackendProcessorCaps> {
            switch (backend_type) {
                case SupportedBackends::Arch_X_86_64:
                    return std::make_unique<back::x86_64::X86InplaceAsm>();
                case SupportedBackends::Arch_ARM_64:
                default:
                    return nullptr;
            }
        }();
    }
}

// executes until programs finish or reach the final text pc
void Layer::execute_program(const std::string &pr_name) {
    if (!programs_list.contains(pr_name)) {
        return;
    }
    read_only.emplace(programs_list.at(pr_name));
    const auto end_pc_offset = read_only->size();
    auto get_pc = [&] {
        return s_guest_f->get_reg(front::AliasRegisters::PC);
    };
    block_linking.pc_set_entry(get_pc());

    do {
        if (compile_cfg_at(get_pc())==nullptr)
            break;

        s_host_b->execute_at_pc(get_pc(), s_guest_f->get_thr_addr());
    } while (get_pc() != end_pc_offset);

    std::cout << std::format("final pc addr: {:X}\n", get_pc());
    std::cout << std::format("unique ir generated: {}\nir statistics:\n", s_guest_f->ir_statistics.size());
    for (const auto & [inst, count] : s_guest_f->ir_statistics) {
        std::cout << std::format("ir {}, count {}\n", to_string(inst), count);
    }
}

void * Layer::compile_cfg_at(uint64_t pc) {
    if (!s_guest_f->is_pc_compiled(pc)) {
        pc = s_guest_f->compile_irs_from_pc(pc); // updates pc, pls save pc before
        const auto &irs_list=s_guest_f->get_irs_from_pc(pc);
        s_host_b->compile_irs(pc, irs_list, block_linking);

        block_linking.verify_or_populate_runtime_jmp_table(s_guest_f->get_table(), read_only->size());
    }

    return s_host_b->pc_x_compiled_block[pc].begin;
}
}
