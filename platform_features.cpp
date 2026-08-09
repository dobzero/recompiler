#include "platform_features.h"

recompiler::PlatformFeatures::PlatformFeatures() {
#if defined(__amd64__)
    a_type_cpu_compiler_target=ProcessorArchType::Arch_X_86_64;

#elif defined(__arm64__)
    a_type_cpu_compiler_target=ProcessorArchType::Arch_ARM_64;

#endif
}
