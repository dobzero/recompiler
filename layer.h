#pragma once
#include "fe/frontend_processor_caps.h"
#include "be/backend_processor_caps.h"
#include "platform_features.h"
#include "block_linking.h"

#include <unordered_map>
#include <cstdint>
#include <memory>

namespace recompiler {
class Layer : public LayerState {
public:
    Layer();
    void ctrl_modify_archs(SupportedFrontends guest_type, SupportedBackends backend_type);
    void create_program(const std::string &pr_name, std::vector< uint8_t> &&ro_program) {
        programs_list.insert_or_assign(pr_name, std::move(ro_program));
    }
    void execute_program(const std::string &program_name);

    void * compile_cfg_at(uint64_t pc);

    PlatformFeatures usable_features_;
private:
    std::unordered_map<std::string, std::vector< uint8_t>> programs_list;

    std::unique_ptr<fe::FrontendProcessorCaps> from_cpu_g;
    std::unique_ptr<be::BackendProcessorCaps> target_cpu_h;

    BlockLinking block_linking;

};
}
