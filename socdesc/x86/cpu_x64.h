#pragma once
#include <optional>

#include "../types.h"

namespace socdesc::x86 {
class Cpu_X64 final : public ProcessorDescriptor {
    public:
    Cpu_X64();

    void set_from_affinity_interval() override;
    [[nodiscard]] std::string to_string() const override;
private:
    std::optional<std::string> modern_brand_name;
};
}
