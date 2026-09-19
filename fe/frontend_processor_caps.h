#pragma once

#include "inplace_vector.h"
#include "../types.h"
namespace recompiler::fe {

class FrontendProcessorCaps {
public:
    virtual ~FrontendProcessorCaps() = default;

    explicit FrontendProcessorCaps(LayerState *top_tier_state, const SupportedFrontends fe_type) : layer_state(top_tier_state), type(fe_type) {}
    void add_supported_backend_targets(const std::initializer_list<SupportedBackends> backends) {
        for (const auto be_type : backends)
            supported_target_cpu_type.emplace_back(be_type);
    }

    virtual bool is_pc_compiled()=0;
    virtual bool compile_irs_from_pc()=0;

    LayerState *layer_state;
    SupportedFrontends type;
    inplace_vector<SupportedBackends, 2> supported_target_cpu_type;
};
}