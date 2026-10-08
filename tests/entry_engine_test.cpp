
#include "asm_isi/arm_v8a_all.h"
#include "layer.h"

namespace recompiler {
namespace {
    class SingleProcessor : public Layer {
    };
}
}

using sf_= recompiler::SupportedFrontends;
using sb_=recompiler::SupportedBackends;
static void testing_armv8_a_2_x86_64(recompiler::SingleProcessor & single_jit) {
    single_jit.ctrl_modify_archs(sf_::Frontend_ARM_V8_A, sb_::Arch_X_86_64);

    const class Arm64PrNops : public asm_isi::ArmV8A_All {
    public:
        Arm64PrNops() {
            for (size_t i =0;i<12;i++)
                NOP();
        }
    } arm64_pr_n;

    single_jit.create_program("NOPS", arm64_pr_n.read_asm_list());
    single_jit.execute_program("NOPS");

}

int main() {
    recompiler::SingleProcessor cpu_multi_arch_jit;

    testing_armv8_a_2_x86_64(cpu_multi_arch_jit);

}
