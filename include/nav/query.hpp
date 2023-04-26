#pragma once
#ifndef NAV__QUERY__HPP
#define NAV__QUERY__HPP

#include <cmath>
#include <functional>

#include "mesh.hpp"
#include "node_arena.hpp"
#include "node_queue.hpp"

class Query {
public:
    Query(std::function<void(const Mesh &, const NodeArena &)> debug_cb) : m_node_arena(1024), m_node_queue(1024), m_debug_cb(debug_cb) {
        
    }

    std::vector<FaceId>
    search(
        const Mesh & mesh,
        FaceId start_face_id,
        Point start_pos,
        FaceId end_face_id,
        Point end_pos
    );

private:
    NodeArena m_node_arena;
    NodeQueue m_node_queue;

    std::function<void(const Mesh &, const NodeArena &)> m_debug_cb;

    
    static
    float
    compute_distance(
        Point a,
        Point b
    ) {
        float d_x = a.m_x - b.m_x;
        float d_y = a.m_y - b.m_y;
        return sqrtf(d_x*d_x + d_y*d_y);
    }

    void
    expand_node(
        const Mesh & mesh,
        Node & node,
        Point end_pos
    );
};

#endif