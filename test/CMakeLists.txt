# Test NodeArena
add_executable(
    test_node_arena
    test_node_arena.cpp
)

target_link_libraries(
    test_node_arena
    PRIVATE
    nav2
)

add_test(NAME test_node_arena COMMAND test_node_arena)

# Test NodeQueue
add_executable(
    test_node_queue
    test_node_queue.cpp
)

target_link_libraries(
    test_node_queue
    PRIVATE
    nav2
)

add_test(NAME test_node_queue COMMAND test_node_queue)