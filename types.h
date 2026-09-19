#pragma once
#include <optional>
#include <cstdint>
#include <span>

namespace recompiler {
struct LayerState {
    std::optional<std::span<const uint8_t>> read_only;

    [[nodiscard]] auto read_u32(const uint64_t p_addr) const {
        const auto &ro_list_span = read_only.value();
        return ro_list_span[p_addr] |
            ro_list_span[p_addr + 1] << 8 |
                ro_list_span[p_addr + 2] << 16 |
                    ro_list_span[p_addr + 3] << 24;
    }

    [[nodiscard]] auto is_reachable(const uint64_t pc) const {
        const auto &ro_list_span = read_only.value();
        return ro_list_span.size() > pc;
    }
};

enum class SupportedFrontends {
    Frontend_ARM_946E_S, // DS
    Frontend_ARM_7TDMI, // DS
    Frontend_ARM_V8_A, // Switch
    Frontend_ARM_32, // Switch
    Frontend_MIPS_I, // PS1 processor
    Frontend_MIPS_IV, // PS2 processor (with s0n1 extensions)
};
using sf_= SupportedFrontends;
enum class SupportedBackends {
    Arch_X_86_64, Arch_ARM_64
};
}
