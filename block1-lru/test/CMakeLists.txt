include_directories(
    ${CHECK_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(itree_test itree.c)
target_link_libraries(itree_test ${CHECK_LIBRARIES})
add_test(itree_test ${CMAKE_CURRENT_BINARY_DIR}/itree_test)
