
#include "arm64/soc_identifier.h"
#include "x86/cpu_x64.h"
#include "socdesc.h"

#include <iostream>
#include <sched.h>
#include <thread>
static std::unique_ptr<socdesc::ProcessorDescriptor> get_cpu_fd() {
#if __ANDROID__
	return std::make_unique<socdesc::arm64::SocIdentifier>();
#else
	return std::make_unique<socdesc::x86::Cpu_X64>();
#endif
}

socdesc::SOC_Desc::SOC_Desc( ) : soc_or_proc(get_cpu_fd()) {
	for (size_t i=0; i < std::thread::hardware_concurrency(); i++) {

		cpu_set_t cpu_set;
		CPU_ZERO(&cpu_set);
		CPU_SET(i, &cpu_set);
		sched_setaffinity(0, sizeof(cpu_set), &cpu_set);


		soc_or_proc->set_from_affinity_interval();
	}
	std::cout << soc_or_proc->to_string() << '\n';

}
