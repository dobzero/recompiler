#pragma once
#include "types.h"

namespace recompiler::be {
class BackendProcessorCaps {
    public:
    explicit BackendProcessorCaps(const SupportedBackends _type) : type(_type) {}
    SupportedBackends type;
};
}
