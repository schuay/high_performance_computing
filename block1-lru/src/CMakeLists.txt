include_directories(
    ${CMAKE_SOURCE_DIR}/uthash/src
)

add_library(stackdist
    hash.c
    itree.c
    stackdist.c
)

add_library(naive
    naive.c
)

add_executable(lru-misses
    lru-misses.c
    naive.c
)

target_link_libraries(lru-misses
    stackdist
)

add_executable(gentrace
    gentrace.c
)
