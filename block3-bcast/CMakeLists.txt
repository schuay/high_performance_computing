cmake_minimum_required(VERSION 2.8.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../CMakeModules/")

project(block3-bcast)

find_package(MPI REQUIRED)
find_package(Check REQUIRED)

set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS} ${MPI_LINK_FLAGS})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE")
endif()

enable_testing()

# Saturn still has old an FindMPI.cmake version without
# MPI_C_LIBRARIES. Hack in a fix here:
if(NOT DEFINED MPI_C_LIBRARIES)
    set(MPI_C_LIBRARIES
        /usr/lib/libmpich.so
        /usr/lib/libopa.a
        /usr/lib/libpthread.so
        /usr/lib/librt.so)
endif()

add_subdirectory(src)
add_subdirectory(test)