#pragma once

#include "face_id.hpp"
#include "point.hpp"

class Node;

enum class NodeEdgeLocation {
    LEFT   = 0,
    MIDDLE = 1,
    RIGHT  = 2,
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
    NodeEdgeLocation m_edge_location;
    NodeState m_state;
};
