#include "layer.h"

#include "fe/v8-a/processor_arm64_v8a.h"
#include "be/x86_64/x86_inplace_asm.h"
namespace recompiler {
Layer::Layer() {

}

// modify how will be the guest and host processor
void Layer::ctrl_modify_archs(const SupportedFrontends guest_type, const SupportedBackends backend_type) {
    if (from_cpu_g && from_cpu_g->type!=guest_type)
        from_cpu_g.reset();
    if (target_cpu_h && target_cpu_h->type!=backend_type)
        from_cpu_g.reset();

    if (!from_cpu_g) {
        switch (guest_type) {
            case SupportedFrontends::Frontend_ARM_V8_A:
                from_cpu_g=std::make_unique<fe::v8_a::ProcessorArm64v8a>();
                break;
            default:
                return;
        }
        // checking compatibility of both mechanism
        if (auto &supported_archs_type = from_cpu_g->supported_target_cpu_type; supported_archs_type[0]!=backend_type||supported_archs_type[1]!=backend_type) {

        }
    }
    if (!target_cpu_h) {
        switch (backend_type) {
            case SupportedBackends::Arch_ARM_64:
                return;
            case SupportedBackends::Arch_X_86_64:
                target_cpu_h=std::make_unique<be::x86_64::X86InplaceAsm>();
        }
    }
}

void Layer::run() const {
    from_cpu_g->is_pc_compiled();
}
}
