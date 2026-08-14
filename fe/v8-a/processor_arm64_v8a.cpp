#include "processor_arm64_v8a.h"

recompiler::fe::v8_a::ProcessorArm64v8a::ProcessorArm64v8a() : FrontendProcessorCaps(sf_::Frontend_ARM_V8_A) {
    add_supported_backend_targets({
        SupportedBackends::Arch_X_86_64}
        );
}

bool recompiler::fe::v8_a::ProcessorArm64v8a::is_pc_compiled() {
    if (cached_pc_region.contains(arm_v8a_state.pc))
        return true;

    return false;
}
