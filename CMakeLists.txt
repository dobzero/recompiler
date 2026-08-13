cmake_minimum_required(VERSION 3.20)
project(recompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")

add_subdirectory(tests)
add_library(recompiler-static STATIC
        layer.cpp
        layer.h
        platform_features.cpp
        platform_features.h

        fe/frontend_processor_caps.h
        be/backend_processor_caps.h

        fe/v8-a/processor_arm64_v8a.cpp
        fe/v8-a/processor_arm64_v8a.h
        be/x86_64/x86_inplace_asm.cpp
        be/x86_64/x86_inplace_asm.h

        ir/types.h

        types.h
        vector_inplace.h
        asm_isi/arm_v8_a.h
        asm_isi/types.h
)
