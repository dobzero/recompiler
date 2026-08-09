#pragma once
#include "platform_features.h"
#include "be/backend_processor_caps.h"
#include "fe/frontend_processor_caps.h"

#include <unordered_map>
#include <cstdint>
#include <memory>
#include <span>

namespace recompiler {
class Layer {
public:
    Layer();
    void ctrl_modify_archs(SupportedFrontends guest_type, SupportedBackends backend_type);
    void set_ro_memory(const std::string &program_name, const std::span<const uint8_t> &ro_program) {
        programs_list.insert_or_assign(program_name, ro_program);
    }

    PlatformFeatures usable_features_;
private:
    std::unordered_map<std::string, std::span<const uint8_t>> programs_list;

    std::unique_ptr<fe::FrontendProcessorCaps> from_cpu_g;
    std::unique_ptr<be::BackendProcessorCaps> target_cpu_h;

};
}
