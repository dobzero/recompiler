#pragma once
#include "../inplace_vector.h"
#include <cstdint>
#include <list>
#include <span>

namespace recompiler::be {
using CodeBuffer = std::list<inplace_vector<uint8_t, 0x4000>>;
class CompiledStream {
public:
    CompiledStream() {
        stream_list.emplace_back();
    }

    template <typename T>
    void write_into(const T & value) {
        write_bytes(std::span<uint8_t>(reinterpret_cast<const uint8_t *>(&value, sizeof(value))));
    }

    CodeBuffer && reuse_buffer();
private:
    void write_bytes(const std::span<uint8_t> & bytes_list);

    CodeBuffer stream_list;
};
}
