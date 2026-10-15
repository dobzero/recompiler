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

        front/arm64_v8a/processor_arm64_v8a.cpp
        front/arm64_v8a/processor_arm64_v8a.h
        front/arm64_v8a/arm64_b_list.cpp
        front/arm64_v8a/arm64_b_list.h
        front/frontend_processor_caps.h

        back/x86_64/x86_inplace_asm.cpp
        back/x86_64/x86_inplace_asm.h
        back/x86_64/x86_registers_allocator.cpp
        back/x86_64/x86_registers_allocator.h
        back/x86_64/x86_recipes/x86_data_flow.h
        back/x86_64/x86_recipes/x86_functions.h
        back/x86_64/x86_recipes/x86_all.h
        back/compiled_stream.cpp
        back/compiled_stream.h
        back/registers_allocator.cpp
        back/registers_allocator.h
        back/backend_processor_caps.cpp
        back/backend_processor_caps.h

        ir/types.h
        ir/default_arch_to_ir.cpp
        ir/ir_cfg.cpp
        ir/ir_builder.cpp
        ir/ir_builder.h

        types.h
        inplace_vector.h
)
target_link_libraries(recompiler-static PUBLIC asm_isi socdesc)
