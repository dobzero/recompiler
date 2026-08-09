
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
static void testing_armv8_a_2_x86_64(recompiler::SingleProcessor & sp) {
    sp.ctrl_modify_archs(sf_::Frontend_ARM_V8_A, sb_::Arch_X_86_64);

    const class Arm64_Nops : public recompiler::asm_isi::ArmV8_A {
    public:
        Arm64_Nops() {
            for (size_t i =0;i<12;i++)
                NOP();
        }
    } arm64_nops;
    // arm64_nops must live as long this program should execute, it's a span not a vector. memory it's in arm64_nops
    sp.set_ro_memory("NOPS for ARMv8-a", arm64_nops.get_asm());
}

int main() {
    recompiler::SingleProcessor cpu_multi_arch_tester;

    testing_armv8_a_2_x86_64(cpu_multi_arch_tester);

}
