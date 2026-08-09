cmake_minimum_required(VERSION 3.20)

enable_testing()

function(recompiler_add_test target desc)
    add_executable(${target} "${ARGN}")
    add_test(${target} ${desc})
    target_link_libraries(${target} PRIVATE recompiler-static)
endfunction()


recompiler_add_test(bunch_of_nops "Bunch of NOPS of all fe-arch, just for testing entries point" bunch_of_nops.cpp)