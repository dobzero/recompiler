#pragma once
#include <memory>
#include "types.h"
namespace socdesc {
class SOC_Desc {
	public:
		SOC_Desc();

	std::unique_ptr<ProcessorDescriptor> soc_or_proc;
};
}
