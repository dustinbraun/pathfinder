#include <cassert>

#include <nav/node_arena.hpp>

int main() {
    NodeArena node_arena(4);
    assert(node_arena.get_size() == 0);
    assert(node_arena.is_empty() == true);
    
    {
        Node * node = node_arena.get_node(0, 1);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 1);
        assert(node != nullptr);
        assert(node->m_prev_face == 0);
        assert(node->m_next_face == 1);
        assert(node->m_state.m_is_open == false);
        assert(node->m_state.m_is_closed == false);
        assert(node->m_state.m_g == 0.0f);
        assert(node->m_state.m_f == 0.0f);
        assert(node->m_state.m_parent_node == nullptr);
        node->m_state.m_is_open = true;
        node->m_state.m_is_closed = true;
        node->m_state.m_g = 1.0f;
        node->m_state.m_f = 2.0f;
    }

    {
        Node * node = node_arena.get_node(1, 0);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 1);
        assert(node != nullptr);
        assert(node->m_prev_face == 0);
        assert(node->m_next_face == 1);
        assert(node->m_state.m_is_open == true);
        assert(node->m_state.m_is_closed == true);
        assert(node->m_state.m_g == 1.0f);
        assert(node->m_state.m_f == 2.0f);
        assert(node->m_state.m_parent_node == nullptr);
    }

    {
        Node * node = node_arena.get_node(1, 1);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 2);
        assert(node != nullptr);
        assert(node->m_prev_face == 1);
        assert(node->m_next_face == 1);
        assert(node->m_state.m_is_open == false);
        assert(node->m_state.m_is_closed == false);
        assert(node->m_state.m_g == 0.0f);
        assert(node->m_state.m_f == 0.0f);
        assert(node->m_state.m_parent_node == nullptr);
    }

    {
        Node * node = node_arena.get_node(1, 2);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 3);
        assert(node != nullptr);
        assert(node->m_prev_face == 1);
        assert(node->m_next_face == 2);
        assert(node->m_state.m_is_open == false);
        assert(node->m_state.m_is_closed == false);
        assert(node->m_state.m_g == 0.0f);
        assert(node->m_state.m_f == 0.0f);
        assert(node->m_state.m_parent_node == nullptr);
        node->m_state.m_is_open = true;
        node->m_state.m_is_closed = true;
        node->m_state.m_g = 2.0f;
        node->m_state.m_f = 3.0f;
    }

    {
        Node * node = node_arena.get_node(1, 3);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 4);
        assert(node != nullptr);
        assert(node->m_prev_face == 1);
        assert(node->m_next_face == 3);
        assert(node->m_state.m_is_open == false);
        assert(node->m_state.m_is_closed == false);
        assert(node->m_state.m_g == 0.0f);
        assert(node->m_state.m_f == 0.0f);
        assert(node->m_state.m_parent_node == nullptr);
    }

    {
        Node * node = node_arena.get_node(1, 4);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 4);
        assert(node == nullptr);
    }

    {
        Node * node = node_arena.get_node(1, 2);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 4);
        assert(node != nullptr);
        assert(node->m_prev_face == 1);
        assert(node->m_next_face == 2);
        assert(node->m_state.m_is_open == true);
        assert(node->m_state.m_is_closed == true);
        assert(node->m_state.m_g == 2.0f);
        assert(node->m_state.m_f == 3.0f);
        assert(node->m_state.m_parent_node == nullptr);
    }

    node_arena.clear();
    assert(node_arena.is_empty() == true);
    assert(node_arena.get_size() == 0);

    {
        Node * node = node_arena.get_node(0, 1);
        assert(node_arena.is_empty() == false);
        assert(node_arena.get_size() == 1);
        assert(node != nullptr);
        assert(node->m_prev_face == 0);
        assert(node->m_next_face == 1);
        assert(node->m_state.m_is_open == false);
        assert(node->m_state.m_is_closed == false);
        assert(node->m_state.m_g == 0.0f);
        assert(node->m_state.m_f == 0.0f);
        assert(node->m_state.m_parent_node == nullptr);
    }
}