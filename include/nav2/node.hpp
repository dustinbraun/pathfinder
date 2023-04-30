#pragma once

#include "face_id.hpp"
#include "point.hpp"
#include "edge_location.hpp"

namespace nav2 {

class Node;

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