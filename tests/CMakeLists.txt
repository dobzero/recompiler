cmake_minimum_required(VERSION 3.20)

enable_testing()

function(recompiler_add_test target desc)
    add_executable(${target} "${ARGN}")
    add_test(${target} ${desc})
    target_link_libraries(${target} PRIVATE recompiler-static)
endfunction()


recompiler_add_test(entry_engine_test "just for test entry points" entry_engine_test.cpp)