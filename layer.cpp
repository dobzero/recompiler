#include "layer.h"

#include "fe/v8-a/processor_arm64_v8a.h"
#include "be/x86_64/x86_inplace_asm.h"
namespace recompiler {
Layer::Layer() {

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
    if (!from_cpu_g->is_pc_compiled()) {
        from_cpu_g->compile_irs_from_pc();
    }

}
}
