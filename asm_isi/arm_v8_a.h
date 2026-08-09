#pragma once
#include <cstdint>

#include "types.h"
namespace recompiler::asm_isi {

class ArmV8_A : protected FixedBufferSizeEmitter<uint32_t> {
public:
    // https://www.scs.stanford.edu/~zyedidia/arm64/nop.html
    uint32_t NOP() {
        return emitter(0xD503201F);
    }
    [[nodiscard]] auto get_asm() const {
        return std::span{reinterpret_cast<const uint8_t *>(asm_buffer_stream.data()), asm_buffer_stream.size()};
    }
};
}
