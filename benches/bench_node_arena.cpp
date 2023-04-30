#include <iostream>

#include <nav2/node_arena.hpp>

#include "profiler.hpp"

using namespace nav2;

int main() {
    Profiler profiler;

    NodeArena node_arena(1024);

    int64_t duration = profiler.run(1000, [&]() {
        node_arena.clear();
    });

    std::cout << duration << " nanoseconds" << std::endl;

    return 0;
}