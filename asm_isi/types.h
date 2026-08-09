#pragma once
#include <cstdint>
#include <span>
#include <vector>

namespace recompiler::asm_isi {
class BufferedEmitter {
public:
    template<typename T>
    auto emitter(const T &value) {
        for (std::span bytes{reinterpret_cast<const uint8_t*>(&value), sizeof(T)}; const auto inst_byte_be:bytes)
            asm_buffer_stream.emplace_back(inst_byte_be);
        return value;
    }
protected:
    std::vector<uint8_t> asm_buffer_stream;
};

template<typename T>
class FixedBufferSizeEmitter {
public:
    auto emitter(const T &value) {
        return asm_buffer_stream.emplace_back(value);
    }
protected:
    std::vector<T> asm_buffer_stream;
};
}
