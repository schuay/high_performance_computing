cmake_minimum_required(VERSION 2.6.4)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../CMakeModules/")
set(CMAKE_PROGRAM_PATH ${CMAKE_PROGRAM_PATH} /opt/NECmpi/gcc/1.2.6/bin64/)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/NECmpi/gcc/1.2.6/lib/)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /opt/NECmpi/gcc/1.2.6/include/)

project(block3-bcast)

find_package(MPI REQUIRED)
find_package(Check)

set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS} ${MPI_LINK_FLAGS})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=c99 -D_GNU_SOURCE")
endif()

# Saturn still has old an FindMPI.cmake version without
# MPI_C_LIBRARIES. Hack in a fix here:
if(NOT DEFINED MPI_C_LIBRARIES)
    set(MPI_C_LIBRARIES
        /opt/NECmpi/gcc/1.2.6/lib64/libmpi.so)
endif()

add_subdirectory(src)

if(CHECK_FOUND)
    enable_testing()
    add_subdirectory(test)
endif()
