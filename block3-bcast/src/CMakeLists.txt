include_directories(
    ${MPI_INCLUDE_PATH}
)

add_library(native native.c)
target_link_libraries(native ${MPI_C_LIBRARIES})

add_library(util util.c)
