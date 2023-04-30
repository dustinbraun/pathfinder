#pragma once

#include <cstdint>

#include "node.hpp"

namespace nav2 {

class NodeId {
public:
    FaceId m_min_face_id;
    FaceId m_max_face_id;

    EdgeLocation m_edge_location;

    NodeId(
        FaceId face_id_a,
        FaceId face_id_b,
        EdgeLocation edge_location
    ) : m_edge_location(edge_location) {
        if (face_id_a <= face_id_b) {
            m_min_face_id = face_id_a;
            m_max_face_id = face_id_b;
        }
        else {
            m_min_face_id = face_id_b;
            m_max_face_id = face_id_a;
        }
    }

    bool
    operator == (
        const NodeId& rhs
    ) const {
        return (m_min_face_id == rhs.m_min_face_id) && (m_max_face_id == rhs.m_max_face_id) && (m_edge_location == rhs.m_edge_location);
    }

    bool
    operator != (
        const NodeId& rhs
    ) const {
        return (m_min_face_id != rhs.m_min_face_id) || (m_max_face_id != rhs.m_max_face_id) || (m_edge_location != rhs.m_edge_location);
    }
};

class NodeArenaBucket {
public:
    uint16_t m_salt; // Indicates when the bucket was created.
    uint16_t m_node_index;
};

// This NodeArena keeps a salt-value to check when a node was created.
// Instead of resetting all buckets with O(n) complexity, we just increment
// the salt value with O(1) complexity.
class NodeArena {
public:
    NodeArena(uint16_t capacity) : m_salt(1), m_capacity(capacity), m_size(0), m_bucket_count(capacity*2), m_buckets(new NodeArenaBucket[m_bucket_count]), m_nodes(new Node[capacity]) {
        for (uint32_t i = 0; i < m_bucket_count; ++i) {
            m_buckets[i].m_salt = 0;
        }
    }

    ~NodeArena() {
        delete[] m_buckets;
        delete[] m_nodes;
    }

    uint16_t get_capacity() const {
        return m_capacity;
    }

    uint16_t get_size() const {
        return m_size;
    }

    bool is_empty() const {
        return m_size == 0;
    }

    const Node * get_nodes() const {
        return m_nodes;
    }

    void clear() {
        if (m_salt == UINT16_MAX) {
            m_salt = 1;
            for (uint32_t i = 0; i < m_bucket_count; ++i) {
                m_buckets[i].m_salt = 0;
            }
        }
        else {
            m_salt++;
        }
        m_size = 0;
    }

    Node *
    get_node(
        uint16_t prev_face_id,
        uint16_t next_face_id,
        EdgeLocation edge_location = EdgeLocation::MIDDLE
    ) {
        NodeId node_id(prev_face_id, next_face_id, edge_location);
        uint32_t hash = compute_hash(node_id);
        uint32_t bucket_index = hash % m_bucket_count;
        while (m_buckets[bucket_index].m_salt == m_salt) {
            Node & node = m_nodes[m_buckets[bucket_index].m_node_index];
            if (NodeId(node.m_prev_face, node.m_next_face, node.m_edge_location) == node_id) {
                return &node;
            }
            bucket_index++;
            if (bucket_index == m_bucket_count) {
                bucket_index = 0;
            }
        }
        if (m_size == m_capacity) {
            return nullptr;
        }
        NodeArenaBucket & bucket = m_buckets[bucket_index];
        bucket.m_salt = m_salt;
        bucket.m_node_index = m_size;
        Node & node = m_nodes[m_size];
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

private:
    uint16_t m_salt;

    uint16_t m_capacity;

    uint16_t m_size;

    uint32_t m_bucket_count;

    NodeArenaBucket * m_buckets;

    Node * m_nodes;


    uint32_t compute_hash(NodeId node_id) {
        return node_id.m_min_face_id ^ node_id.m_max_face_id ^ static_cast<uint32_t>(node_id.m_edge_location);
    }
};

} // namespace nav2
