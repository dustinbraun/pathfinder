#pragma once

#include "mesh.hpp"
#include "node.hpp"

class NodeId {
public:
    FaceId m_min_face_id;
    FaceId m_max_face_id;

    NodeId(
        FaceId face_id_a,
        FaceId face_id_b
    ) {
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
        return (m_min_face_id == rhs.m_min_face_id) && (m_max_face_id == rhs.m_max_face_id);
    }

    bool
    operator != (
        const NodeId& rhs
    ) const {
        return (m_min_face_id != rhs.m_min_face_id) || (m_max_face_id != rhs.m_max_face_id);
    }
};

class NodeArena {
public:
    NodeArena(uint16_t capacity) : m_capacity(capacity), m_size(0), m_node_indices(new uint16_t[capacity*2]), m_nodes(new Node[capacity]) {
        for (size_t i = 0; i < m_capacity*2; ++i) {
            m_node_indices[i] = 0xFFFF;
        }
    }

    void
    clear() {
        for (size_t i = 0; i < m_capacity*2; ++i) {
            m_node_indices[i] = 0xFFFF;
        }
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

    size_t
    get_capacity() const {
        return m_capacity;
    }

    Node *
    get_node(
        FaceId prev_face_id,
        FaceId next_face_id
    );

    const Node *
    get_nodes() const {
        return m_nodes;
    }

private:
    Node * m_nodes;

    uint16_t * m_node_indices;

    uint16_t m_capacity;

    uint16_t m_size;


    uint16_t get_hash(
        NodeId node_id
    ) {
        // ToDo: More sophisticating hashing function.
        return (static_cast<uint16_t>(node_id.m_min_face_id) ^ static_cast<uint16_t>(node_id.m_max_face_id)) % (m_capacity*2);
    }
};