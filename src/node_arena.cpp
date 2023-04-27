#include "nav/node_arena.hpp"

Node *
NodeArena::get_node(
    FaceId prev_face_id,
    FaceId next_face_id,
    NodeEdgeLocation edge_location
) {
    NodeId node_id(prev_face_id, next_face_id, edge_location);
    uint16_t hash = get_hash(node_id);
    while (m_node_indices[hash] != 0xFFFF) {
        Node & node = m_nodes[m_node_indices[hash]];
        if (NodeId(node.m_prev_face, node.m_next_face, node.m_edge_location) == node_id) {
            return &node;
        }
        hash++;
        if (hash == m_capacity*2) {
            hash = 0;
        }
    }
    if (m_size == m_capacity) {
        return nullptr;
    }
    m_node_indices[hash] = m_size;
    Node & node = m_nodes[static_cast<size_t>(m_size)];
    node.m_prev_face = prev_face_id;
    node.m_next_face = next_face_id;
    node.m_edge_location = edge_location;
    node.m_state.m_pos.m_x = 0.0f;
    node.m_state.m_pos.m_y = 0.0f;
    node.m_state.m_is_open = false;
    node.m_state.m_is_closed = false;
    node.m_state.m_g = 0.0f;
    node.m_state.m_f = 0.0f;
    node.m_state.m_parent_node = nullptr;
    m_size++;
    return &node;
}