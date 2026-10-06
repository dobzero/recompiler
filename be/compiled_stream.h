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
    CompiledStream(const CompiledStream&)=delete;
    CompiledStream(CompiledStream &&movecs) noexcept {
        stream_list = std::move(movecs.stream_list);
    }

    template <typename T>
    void write_into(const T & value) {
        write_bytes(std::span(reinterpret_cast<const uint8_t *>(&value), sizeof(value)));
    }
    void write_bytes(const std::initializer_list<uint8_t> &bytes_list) {
        write_bytes(std::span(bytes_list));
    }

    CodeBuffer && reuse_buffer();
    auto for_loop_stream(auto &&callback) const {
        for (const auto &iv_code : stream_list) {
            callback(iv_code);
        }
    }
    auto get_stream_size() const {
        size_t size = 0;
        for_loop_stream([&](const auto &iv) {
            size += iv.size();
        });
        return size;
    }
private:
    void write_bytes(const std::span<const uint8_t> & bytes_list);

    CodeBuffer stream_list;
};
}
