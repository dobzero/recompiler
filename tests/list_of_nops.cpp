
#include "asm_isi/arm_v8_a.h"
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

    const class Arm64_Testing : public asm_isi::ArmV8_A {
    public:
        Arm64_Testing() {
            for (size_t i =0;i<12;i++)
                __NOP();
        }
    } amr64_instruction_container;

    single_jit.create_program("nops.arm", amr64_instruction_container.get_asm());
    single_jit.execute_program("nops.arm");

}

int main() {
    recompiler::SingleProcessor cpu_multi_arch_jit;

    testing_armv8_a_2_x86_64(cpu_multi_arch_jit);

}
