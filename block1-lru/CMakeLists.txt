cmake_minimum_required(VERSION 2.8.0)

project(block1-lru)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../CMakeModules/")

find_package(Check)
find_package(Doxygen)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions("-Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=199506")
endif()

if(DOXYGEN_FOUND)
    add_subdirectory(doc)
endif()

add_subdirectory(src)

if(CHECK_FOUND)
    enable_testing()
    add_subdirectory(test)
endif()
