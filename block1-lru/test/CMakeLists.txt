include_directories(
    ${CHECK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/uthash/src
)

add_executable(itree_test itree.c ${CMAKE_SOURCE_DIR}/src/itree.c)
target_link_libraries(itree_test ${CHECK_LIBRARIES})
add_test(itree_test ${CMAKE_CURRENT_BINARY_DIR}/itree_test)

add_executable(hash_test hash.c ${CMAKE_SOURCE_DIR}/src/hash.c)
target_link_libraries(hash_test ${CHECK_LIBRARIES})
add_test(hash_test ${CMAKE_CURRENT_BINARY_DIR}/hash_test)

add_executable(stackdist_test stackdist.c ${CMAKE_SOURCE_DIR}/src/stackdist.c)
target_link_libraries(stackdist_test ${CHECK_LIBRARIES} stackdist)
add_test(stackdist_test ${CMAKE_CURRENT_BINARY_DIR}/stackdist_test)
