cmake_minimum_required(VERSION 3.20)
project(recompiler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")

include_directories(".")

add_subdirectory(tests)
add_subdirectory(asm_isi)
add_subdirectory(socdesc)
add_library(recompiler-static STATIC
        layer.cpp
        layer.h
        platform_features.cpp
        platform_features.h
        block_linking.cpp
        block_linking.h

        fe/v8-a/processor_arm64_v8a.cpp
        fe/v8-a/processor_arm64_v8a.h
        fe/frontend_processor_caps.h
        be/backend_processor_caps.h

        be/x86_64/x86_inplace_asm.cpp
        be/x86_64/x86_inplace_asm.h
        be/x86_64/x86_registers_allocator.cpp
        be/x86_64/x86_registers_allocator.h
        be/x86_64/x86_recipes/x86_data_flow.h
        be/x86_64/x86_recipes/x86_functions.h
        be/x86_64/x86_recipes/x86_all.h
        be/compiled_stream.cpp
        be/compiled_stream.h
        be/registers_allocator.cpp
        be/registers_allocator.h
        be/backend_processor_caps.cpp

        ir/types.h
        ir/default_arch_to_ir.cpp
        ir/ir_cfg.cpp

        types.h
        inplace_vector.h
)
target_link_libraries(recompiler-static PUBLIC asm_isi socdesc)
