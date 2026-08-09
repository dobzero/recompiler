#include "processor_arm64_v8a.h"

recompiler::fe::v8_a::ProcessorArm64v8a::ProcessorArm64v8a() : FrontendProcessorCaps(sf_::Frontend_ARM_V8_A) {
    add_supported_backend_targets({
        SupportedBackends::Arch_X_86_64}
        );
}
