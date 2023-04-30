#pragma once

#include "face_id.hpp"
#include "point.hpp"
#include "edge_location.hpp"

namespace nav2 {

class Node;

class NodeId {
public:
    FaceId m_prev_face_id;
    FaceId m_next_face_id;

    NodeId(
        FaceId prev_face_id,
        FaceId next_face_id
    ) : m_prev_face_id(prev_face_id), m_next_face_id(next_face_id) {
        
    }

    bool
    operator == (
        const NodeId& rhs
    ) const {
        if ((m_prev_face_id == rhs.m_prev_face_id) && (m_next_face_id == rhs.m_next_face_id)) {
            return true;
        }
        if ((m_prev_face_id == rhs.m_next_face_id) && (m_next_face_id == rhs.m_prev_face_id)) {
            return true;
        }
        return false;
    }
};

class NodeState {
public:
    Point m_pos;
    bool m_is_open;
    bool m_is_closed;
    float m_g;
    float m_f;
    Node * m_parent_node;
};

class Node {
public:
    FaceId m_prev_face;
    FaceId m_next_face;
    EdgeLocation m_edge_location;
    NodeState m_state;
};

} // namespace nav2
