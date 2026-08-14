#include "x86_inplace_asm.h"

#include <stdexcept>
#include <format>

#include "be/compiled_stream.h"

class recompiler_exception : public std::runtime_error {
public:
    template <typename ...Args>
    explicit recompiler_exception(const std::format_string<Args...> &fmt_args={}, Args&& ...args) : std::runtime_error(
        std::format(fmt_args, std::forward<Args>(args)...)) {}
};

namespace recompiler::be::x86_64 {
X86InplaceAsm::X86InplaceAsm() : BackendProcessorCaps(SupportedBackends::Arch_X_86_64) {

}
}

void recompiler::be::x86_64::X86InplaceAsm::compile_irs(const uint64_t pc_, const std::vector<ir::Ir> &irs_ops) {

    CompiledStream x86_result;
    if (pc_!=0) {}

    for (const auto &ir_op : irs_ops) {
        if (ir_op.is_exclusive_of_arch_type)
            if (ir_op.is_exclusive_of_arch_type != type)
                throw recompiler_exception("this IR requires a specific backend");
        switch (ir_op.type) {
            case ir::IrOperationType::Ir_NOP_OP:

            default: {}
        }
    }
}
