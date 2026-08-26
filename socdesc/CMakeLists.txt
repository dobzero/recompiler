cmake_minimum_required(VERSION 4.0)

project(socdesc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOCDESC_SRC socdesc.cpp socdesc.h types.h)
if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
    list(APPEND SOCDESC_SRC x86/cpu_x64.cpp x86/cpu_x64.h)
else()
    list(APPEND SOCDESC_SRC arm64/soc_identifier.cpp arm64/soc_identifier.h)
endif()

add_library(socdesc STATIC ${SOCDESC_SRC})

add_executable(test test.cpp)
target_link_libraries(test PRIVATE socdesc)
