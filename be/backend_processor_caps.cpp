
#include <sys/mman.h>
#include "backend_processor_caps.h"

#include "compiled_stream.h"

recompiler::be::CompiledBlockDetails recompiler::be::enable_jit_in(const CompiledStream &compiled) {
    const auto code_size = [&compiled] {
        const auto size = compiled.get_stream_size();
        return (size + 4095) & ~4095;
    }();
    void * code = mmap(nullptr, code_size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    size_t pc_offset_size=0;
    compiled.for_loop_stream([&](const auto &code_chunks) {
        memcpy(static_cast<uint8_t*>(code)+pc_offset_size, code_chunks.data(), code_chunks.size());
        pc_offset_size += code_chunks.size();
    });
    return CompiledBlockDetails{
        .begin = code,
        .size = code_size,
        .pc_after = pc_offset_size,
    };
}

void recompiler::be::disable_jit_in(CompiledBlockDetails &block) {
    munmap(block.begin, block.size);
    block = {};
}
