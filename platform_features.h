#pragma once
#include "socdesc/socdesc.h"
#include "types.h"

namespace recompiler {
enum class ProcessorFeatures {

};
using ProcessorArchType=SupportedBackends;

class PlatformFeatures {
public:
    PlatformFeatures();

    ProcessorArchType a_type_cpu_compiler_target;
    socdesc::SOC_Desc soc_description;

};
}
