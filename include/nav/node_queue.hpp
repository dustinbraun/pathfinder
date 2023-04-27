#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>

#include "node.hpp"

class NodeQueue {
public:
    NodeQueue(
        size_t capacity
    ) : m_capacity(capacity), m_size(0), m_nodes(new Node * [capacity]) {

    }

    void clear() {
        m_size = 0;
    }

    bool
    is_empty() const {
        return m_size == 0;
    }

    size_t
    get_size() const {
        return m_size;
    }

    void
    insert_node(
        Node * node
    ) {
        assert(m_size != m_capacity);
        size_t current_index = m_size;
        m_size++;
        m_nodes[current_index] = node;
        while (current_index != 0 && m_nodes[get_parent_index(current_index)]->m_state.m_f > m_nodes[current_index]->m_state.m_f) {
            std::swap(m_nodes[get_parent_index(current_index)], m_nodes[current_index]);
            current_index = get_parent_index(current_index);
        }
    }

    void
    modify_node(
        Node & node
    ) {
        size_t current_index = 0;
        assert(current_index < m_size);
        while (m_nodes[current_index] != &node) {
            assert(current_index < m_size);
            current_index++;
        }
        while (current_index != 0 && m_nodes[get_parent_index(current_index)]->m_state.m_f > m_nodes[current_index]->m_state.m_f) {
            std::swap(m_nodes[get_parent_index(current_index)], m_nodes[current_index]);
            current_index = get_parent_index(current_index);
        }
    }

    Node &
    pop_node() {
        assert(m_size > 0);
        if (m_size == 1) {
            m_size--;
            return (*m_nodes[0]);
        }
        Node * min_node = m_nodes[0];
        m_nodes[0] = m_nodes[m_size - 1];
        m_size--;
        heapify(0);
        return (*min_node);
    }

    void
    print_nodes() {
        for (size_t i = 0; i < m_size; ++i) {
            std::cout << m_nodes[i]->m_state.m_f << " ";
        }
        std::cout << std::endl;
    }

private:
    size_t m_capacity;

    size_t m_size;

    Node ** m_nodes;


    static
    size_t
    get_parent_index(
        size_t current_index
    ) {
        return (current_index - 1)/2;
    }

    static
    size_t
    get_left_child_index(
        size_t current_index
    ) {
        return (2*current_index) + 1;
    }

    static
    size_t
    get_right_child_index(
        size_t current_index
    ) {
        return (2*current_index) + 2;
    }

    void
    heapify(
        size_t current_index
    ) {
        size_t l = get_left_child_index(current_index);
        size_t r = get_right_child_index(current_index);
        size_t smallest = current_index;
        if (l < m_size && m_nodes[l]->m_state.m_f < m_nodes[smallest]->m_state.m_f) {
            smallest = l;
        }
        if (r < m_size && m_nodes[r]->m_state.m_f < m_nodes[smallest]->m_state.m_f) {
            smallest = r;
        }
        if (smallest != current_index) {
            std::swap(m_nodes[current_index], m_nodes[smallest]);
            heapify(smallest);
        }
    }
};