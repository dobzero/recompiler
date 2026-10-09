#pragma once

#include "inplace_vector.h"
#include "ir/types.h"
#include <vector>
namespace recompiler::fe {
enum class AliasRegisters {
    PC, Linker
};

class FrontendProcessorCaps {
public:
    virtual ~FrontendProcessorCaps() = default;


    explicit FrontendProcessorCaps(LayerState *top_tier_state, const SupportedFrontends fe_type) : layer_state(top_tier_state), type(fe_type) {}
    void add_supported_backend_targets(const std::initializer_list<SupportedBackends> backends) {
        for (const auto be_type : backends)
            supported_target_cpu_type.emplace_back(be_type);
    }

    virtual void * get_thr_addr()=0;
    virtual bool is_pc_compiled(uint64_t)=0;
    // return the first pc value
    virtual uint64_t compile_irs_from_pc(uint64_t)=0;
    virtual std::vector<ir::Ir> get_irs_from_pc(uint64_t pc)=0;

    virtual uint64_t get_reg(const AliasRegisters &reg)=0;

    LayerState *layer_state;
    SupportedFrontends type;
    inplace_vector<SupportedBackends, 2> supported_target_cpu_type;
};
}