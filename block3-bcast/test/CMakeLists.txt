include_directories(
    ${CHECK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(bcast_test bcast.c)
target_link_libraries(bcast_test
    native
    util
    ${CHECK_LIBRARIES}
    ${MPI_C_LIBRARIES}
)

add_test(bcast_test
    ${MPIEXEC} -x PATH ${MPIEXEC_NUMPROC_FLAG} 4 ${CMAKE_CURRENT_BINARY_DIR}/bcast_test
)
