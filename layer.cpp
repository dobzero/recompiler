#include "layer.h"

#include "fe/arm64_v8a/processor_arm64_v8a.h"
#include "be/x86_64/x86_inplace_asm.h"

extern "C" void * layer_stub_function(void * layer_ptr, const uint64_t pc) {
    auto * layer = static_cast<recompiler::Layer *>(layer_ptr);
    return layer->compile_cfg_at(pc);
}

namespace recompiler {
Layer::Layer() : block_linking(this, layer_stub_function) {
}

// modify what will be the guest and host processor
void Layer::ctrl_modify_archs(const SupportedFrontends guest_type, const SupportedBackends backend_type) {
    if (!guestpr || guestpr->type!=guest_type) {
        guestpr=[&guest_type, this]() -> std::unique_ptr<fe::FrontendProcessorCaps> {
            switch (guest_type) {
                case SupportedFrontends::Frontend_ARM_V8_A:
                    return std::make_unique<fe::arm64_v8a::ProcessorArm64v8a>(this);
                    break;
                default:
                    return nullptr;
            }
        }();
    }
    if (!hostpr || hostpr->type!=backend_type) {
        hostpr=[&backend_type]() -> std::unique_ptr<be::BackendProcessorCaps> {
            switch (backend_type) {
                case SupportedBackends::Arch_X_86_64:
                    return std::make_unique<be::x86_64::X86InplaceAsm>();
                case SupportedBackends::Arch_ARM_64:
                default:
                    return nullptr;
            }
        }();
    }
}

    // executes until program ends
void Layer::execute_program(const std::string &pr_name) {
    if (!programs_list.contains(pr_name)) {
        return;
    }
    read_only.emplace(programs_list.at(pr_name));
    const auto end_pc_offset = read_only->size();
    auto get_pc = [this] {
        return guestpr->get_reg(fe::AliasRegisters::PC);
    };
    block_linking.pc_set_entry(get_pc());

    do {
        if (compile_cfg_at(get_pc())==nullptr)
            break;

        hostpr->execute_at_pc(get_pc(), guestpr->get_thr_addr());
    } while (get_pc() != end_pc_offset);
}

void * Layer::compile_cfg_at(uint64_t pc) {
    if (!guestpr->is_pc_compiled(pc)) {
        pc = guestpr->compile_irs_from_pc(pc); // updates pc, pls save pc before
        const auto &irs_list=guestpr->get_irs_from_pc(pc);
        hostpr->compile_irs(pc, irs_list, block_linking);
    }

    return hostpr->pc_x_compiled_block[pc].begin;
}
}
