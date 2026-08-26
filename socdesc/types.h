#pragma once

#include <string>
namespace socdesc {
class ProcessorDescriptor {
public:
    virtual ~ProcessorDescriptor() = default;
    virtual void set_from_affinity_interval()=0;

    [[nodiscard]] virtual std::string to_string() const {
        return "not identified";
    }
};
}