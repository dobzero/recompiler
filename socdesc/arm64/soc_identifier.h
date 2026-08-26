#pragma once
#include <optional>
#include <vector>

#include "../types.h"

namespace socdesc::arm64 {

enum class Cortex_Model_Types {
    CORTEX_MODEL_A510=0xD46,
    CORTEX_MODEL_A710=0xD47,
    CORTEX_MODEL_X2=0xD48
};

using C_M=Cortex_Model_Types;

class SocIdentifier final : public ProcessorDescriptor {
public:

    void set_from_affinity_interval() override;
    [[nodiscard]] std::string to_string() const override;
private:
    std::vector<C_M> v_cortex_by_core;
    std::optional<std::string> soc_midr_name;
};
}
