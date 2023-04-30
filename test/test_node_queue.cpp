#include <cassert>

#include <nav/node_queue.hpp>

using namespace nav2;

int main() {
    NodeQueue queue(128);
    assert(queue.is_empty());
    assert(queue.get_size() == 0);

    Node nodes[10];
    nodes[0].m_state.m_f = 2.0f;
    nodes[1].m_state.m_f = 3.0f;
    nodes[2].m_state.m_f = 1.0f;
    nodes[3].m_state.m_f = 5.0f;
    nodes[4].m_state.m_f = 8.0f;
    nodes[5].m_state.m_f = 4.0f;
    nodes[6].m_state.m_f = 9.0f;
    nodes[7].m_state.m_f = 7.0f;
    nodes[8].m_state.m_f = 0.0f;
    nodes[9].m_state.m_f = 6.0f;

    for (size_t i = 0; i < 10; ++i) {
        queue.insert_node(&nodes[i]);
    }
    assert(!queue.is_empty());
    assert(queue.get_size() == 10);

    nodes[5].m_state.m_f = 1.5f;
    queue.modify_node(nodes[5]);
    assert(!queue.is_empty());
    assert(queue.get_size() == 10);

    queue.print_nodes();

    assert(queue.pop_node().m_state.m_f == 0.0f);
    assert(queue.pop_node().m_state.m_f == 1.0f);
    assert(queue.pop_node().m_state.m_f == 1.5f);
    assert(queue.pop_node().m_state.m_f == 2.0f);
    assert(queue.pop_node().m_state.m_f == 3.0f);
    assert(queue.pop_node().m_state.m_f == 5.0f);
    assert(queue.pop_node().m_state.m_f == 6.0f);
    assert(queue.pop_node().m_state.m_f == 7.0f);
    assert(queue.pop_node().m_state.m_f == 8.0f);
    assert(queue.pop_node().m_state.m_f == 9.0f);
    assert(queue.is_empty());
    assert(queue.get_size() == 0);
}