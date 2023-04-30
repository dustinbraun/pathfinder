add_executable(bench_node_arena bench_node_arena.cpp)

target_link_libraries(bench_node_arena PRIVATE nav2)


add_executable(bench_query bench_query.cpp)

target_link_libraries(bench_query PRIVATE nav2)