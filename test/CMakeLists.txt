# add_executable(test_enqueued_node test_enqueued_node.cpp)
# target_link_libraries(test_enqueued_node PRIVATE nav)

# add_executable(test_dequeued_node test_enqueued_node.cpp)
# target_link_libraries(test_dequeued_node PRIVATE nav)


# Test NodeArena
add_executable(
    test_node_arena
    test_node_arena.cpp
)

target_link_libraries(
    test_node_arena
    PRIVATE
    pathfinder
)

# Test NodeQueue
add_executable(
    test_node_queue
    test_node_queue.cpp
)

target_link_libraries(
    test_node_queue
    PRIVATE
    pathfinder
)