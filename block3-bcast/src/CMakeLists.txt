include_directories(
    ${MPI_INCLUDE_PATH}
)

add_library(bcast binary.c binomial.c linear.c native.c)
target_link_libraries(bcast ${MPI_C_LIBRARIES})

add_library(util util.c)

add_executable(bcast_bench bench.c)
target_link_libraries(bcast_bench bcast util ${MPI_C_LIBRARIES})
