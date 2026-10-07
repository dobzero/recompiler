#include "layer.h"

#include "fe/v8-a/processor_arm64_v8a.h"
#include "be/x86_64/x86_inplace_asm.h"

extern "C" void * layer_stub_function(void * layer_ptr, const uint64_t pc) {
    auto * layer = static_cast<recompiler::Layer *>(layer_ptr);
    return layer->compile_cfg_at(pc);
}

namespace recompiler {
Layer::Layer() : block_linking(this, layer_stub_function) {
}

// modify how will be the guest and host processor
void Layer::ctrl_modify_archs(const SupportedFrontends guest_type, const SupportedBackends backend_type) {
    if (!from_cpu_g || from_cpu_g->type!=guest_type) {
        from_cpu_g=[&guest_type, this]() -> std::unique_ptr<fe::FrontendProcessorCaps> {
            switch (guest_type) {
                case SupportedFrontends::Frontend_ARM_V8_A:
                    return std::make_unique<fe::v8_a::ProcessorArm64v8a>(this);
                    break;
                default:
                    return nullptr;
            }
        }();
    }
    if (!target_cpu_h || target_cpu_h->type!=backend_type) {
        target_cpu_h=[&backend_type]() -> std::unique_ptr<be::BackendProcessorCaps> {
            switch (backend_type) {
                case SupportedBackends::Arch_X_86_64:
                    return std::make_unique<be::x86_64::X86InplaceAsm>();
                case SupportedBackends::Arch_ARM_64:
                default:
                    return nullptr;
            }
        }();
    }

    // checking compatibility of both mechanism
    if (auto &supported_archs_type = from_cpu_g->supported_target_cpu_type; supported_archs_type[0]!=backend_type||supported_archs_type[1]!=backend_type) {
    }
}

void Layer::execute_program(const std::string &program_name) {
    if (!programs_list.contains(program_name)) {
        return;
    }
    read_only.emplace(programs_list.at(program_name));
    const auto end_pc_offset = read_only->size();
    size_t pc=0;
    do {
        pc=from_cpu_g->get_reg(fe::AliasRegisters::PC);
        if (compile_cfg_at(pc)==nullptr)
            break;

        pc += target_cpu_h->execute_at_pc(pc, from_cpu_g->get_thr_addr());
    } while (from_cpu_g->get_reg(fe::AliasRegisters::PC) != end_pc_offset);
}

void * Layer::compile_cfg_at(uint64_t pc) {
    if (!from_cpu_g->is_pc_compiled()) {
        pc = from_cpu_g->compile_irs_from_pc(); // updates pc, pls save pc before
        const auto &irs_list=from_cpu_g->get_irs_from_pc(pc);
        target_cpu_h->compile_irs(pc, irs_list, block_linking);
    }

    return target_cpu_h->pc_x_compiled_block[pc].begin;
}
}
