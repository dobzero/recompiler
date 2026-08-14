#include "compiled_stream.h"

namespace recompiler::be {


CodeBuffer && CompiledStream::reuse_buffer() {
    for (auto & vector : stream_list) {
        vector.clear();
    }
    return std::move(stream_list);
}

void CompiledStream::write_bytes(const std::span<uint8_t> & bytes_list) {
    if (stream_list.back().size()+bytes_list.size()>0x80000)
        stream_list.emplace_back();
    auto &bytes_vector=stream_list.back();

    for (const auto &bytes_value : bytes_list) {
        bytes_vector.emplace_back(bytes_value);
    }
}
}