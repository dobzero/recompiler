#include "soc_identifier.h"

#include <unordered_map>
#include <cstdint>
#include <format>
#include <string>
#include <ranges>

namespace socdesc::arm64 {

static const std::unordered_map<uint64_t, const char *> upper_soc_midr {
    {0x410FD4, "Snapdragon 7 Plus Gen 2"} // first cluster
};

void SocIdentifier::set_from_affinity_interval() {
    uint64_t id_aa64pfr0_el1=0;
    uint64_t midr_el1=0;

    __asm(
        "mrs %0, ID_AA64PFR0_EL1\n"
        "mrs %1, MIDR_EL1\n" :
        "=r" (id_aa64pfr0_el1),
        "=r" (midr_el1) : );

    if (auto soc_midr_it = upper_soc_midr.find(midr_el1>>8); !soc_midr_name) {
        if (soc_midr_it!=upper_soc_midr.end()) {
            soc_midr_name.emplace(soc_midr_it->second);
        }
    }

    v_cortex_by_core.push_back(static_cast<C_M>((midr_el1>>4)&0xFFF));
}
std::string SocIdentifier::to_string() const {
    std::string result;
    const auto &midr_name=soc_midr_name.value_or("Unk.");
    result+=std::format("{}\n", midr_name);

    for (size_t i=0; i<v_cortex_by_core.size(); i++) {
        switch (v_cortex_by_core[i]) {
            case C_M::CORTEX_MODEL_A510:
                result+=std::format("{}: Cortex A510\n", i); break;
            case C_M::CORTEX_MODEL_A710:
                result+=std::format("{}: Cortex A710\n", i); break;
            case C_M::CORTEX_MODEL_X2:
                result+=std::format("{}: Cortex X2\n", i); break;
            default:
                result+=std::format("{}: Unk.\n", i);
        }
    }
    return result;
}
}
