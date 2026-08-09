#pragma once

#include "vector_inplace.h"
#include "../types.h"
namespace recompiler::fe {
class FrontendProcessorCaps {
public:
    explicit FrontendProcessorCaps(const SupportedFrontends fe_type) : type(fe_type) {}
    void add_supported_backend_targets(const std::initializer_list<SupportedBackends> backends) {
        for (const auto be_type : backends)
            supported_target_cpu_type.emplace_back(be_type);
    }

    SupportedFrontends type;
    vector_inplace<SupportedBackends, 2> supported_target_cpu_type;
};
}