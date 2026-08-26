
#include "cpu_x64.h"
#include <cpuid.h>
#include <cstdint>
#include <cstring>

socdesc::x86::Cpu_X64::Cpu_X64() {
    char brand_86_64[50]={};
    uint32_t cpuid_vals[4];
    for (const auto leafs:{0x80000002, 0x80000003, 0x80000004}) {
        __get_cpuid(leafs, &cpuid_vals[0], &cpuid_vals[1], &cpuid_vals[2], &cpuid_vals[3]);

        memcpy(brand_86_64+(leafs-0x80000002)*16, cpuid_vals, sizeof(cpuid_vals));
    }
    size_t size=49;
    for (; size; size--)
        if (brand_86_64[size]!=' '&&brand_86_64[size]!='\0')
            break;
    if (size)
        modern_brand_name.emplace(brand_86_64, size+1);
}

void socdesc::x86::Cpu_X64::set_from_affinity_interval() {

}

std::string socdesc::x86::Cpu_X64::to_string() const {
    if (modern_brand_name)
        return *modern_brand_name;
    return ProcessorDescriptor::to_string();
}
