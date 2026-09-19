cmake_minimum_required(VERSION 3.20)
project(recompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(".")

add_subdirectory(tests)
add_subdirectory(asm_isi)
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
        be/compiled_stream.cpp
        be/compiled_stream.h

        ir/types.h
        ir/default_arch_to_ir.cpp
        ir/ir_cfg.cpp

        types.h
        inplace_vector.h
)
target_link_libraries(recompiler-static PUBLIC asm_isi)
