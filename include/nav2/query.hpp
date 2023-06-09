#pragma once

#include <cmath>
#include <functional>

#include "mesh.hpp"
#include "node_arena.hpp"
#include "node_queue.hpp"

namespace nav2 {

class Query {
public:
    Query(std::function<void(const Mesh &, const NodeArena &)> debug_cb) : m_node_arena(1024), m_node_queue(1024), m_debug_cb(debug_cb) {
        
    }

    // Returns a path from start to end.
    // If no path is found, a path to the closest face to the end position
    // is returned.
    std::vector<FaceId>
    search(
        const Mesh & mesh,
        FaceId start_face_id,
        Point start_pos,
        FaceId end_face_id,
        Point end_pos
    ) {
        m_node_arena.clear();
        m_node_queue.clear();
        if (start_face_id == end_face_id) {
            std::vector<FaceId> path;
            path.push_back(end_face_id);
            return path;
        }
        const Face & start_face = mesh.get_face_by_id(start_face_id);
        for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
            FaceId next_face_id = start_face.m_adj_ids[edge_index];
            if (next_face_id != FACE_ID_NONE) {
                Node * node = m_node_arena.get_node(start_face_id, next_face_id);
                if (node != nullptr) {
                    node->m_state.m_pos = mesh.get_edge_center_point(start_face, edge_index); //mesh.get_edge_center_point(start_face, edge_index);
                    node->m_state.m_g = compute_distance(start_pos, node->m_state.m_pos);
                    node->m_state.m_f = node->m_state.m_g + compute_distance(node->m_state.m_pos, end_pos);
                    node->m_state.m_is_open = true;
                    m_node_queue.insert_node(node);

                    // DEBUG
                    m_debug_cb(mesh, m_node_arena);
                }
            }
        }

        Node * best_node = nullptr;
        float best_node_distance;

        while (!m_node_queue.is_empty()) {
            Node & node = m_node_queue.pop_node();
            if (node.m_id.m_next_face_id == end_face_id) {
                return generate_path(node);
            }
            node.m_state.m_is_open = false;
            node.m_state.m_is_closed = true;

            if (best_node == nullptr) {
                best_node = &node;
                best_node_distance = best_node->m_state.m_pos.get_distance(end_pos);
            }
            else {
                float distance = node.m_state.m_pos.get_distance(end_pos);
                if (distance < best_node_distance) {
                    best_node = &node;
                    best_node_distance = distance;
                }
            }
            
            // DEBUG
            m_debug_cb(mesh, m_node_arena);
            expand_node(mesh, node, end_pos);
        }

        if (best_node != nullptr) {
            return generate_path((*best_node));
        }

        return std::vector<FaceId>();
    }

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
    ) {
        for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
            FaceId next_face_id = mesh.get_face_by_id(node.m_id.m_next_face_id).m_adj_ids[edge_index];
            if (next_face_id != FACE_ID_NONE && next_face_id != node.m_id.m_prev_face_id) {
                Node * next_node = m_node_arena.get_node(node.m_id.m_next_face_id, next_face_id);
                if (next_node != nullptr && !next_node->m_state.m_is_closed) {
                    if (next_node->m_state.m_is_open) {
                        float g = node.m_state.m_g + compute_distance(node.m_state.m_pos, next_node->m_state.m_pos);
                        if (g < next_node->m_state.m_g) {
                            next_node->m_state.m_g = g;
                            next_node->m_state.m_f = g + compute_distance(next_node->m_state.m_pos, end_pos);
                            next_node->m_state.m_parent_node = &node;
                            m_node_queue.modify_node((*next_node));
                        }
                    }
                    else {
                        next_node->m_state.m_pos = mesh.get_edge_center_point(mesh.get_face_by_id(node.m_id.m_next_face_id), edge_index);
                        next_node->m_state.m_g = node.m_state.m_g + compute_distance(node.m_state.m_pos, next_node->m_state.m_pos);;
                        next_node->m_state.m_f = next_node->m_state.m_g + compute_distance(next_node->m_state.m_pos, end_pos);
                        next_node->m_state.m_parent_node = &node;
                        next_node->m_state.m_is_open = true;
                        m_node_queue.insert_node(next_node);
                    }

                    // DEBUG
                    m_debug_cb(mesh, m_node_arena);
                }
            }
        }
    }

    std::vector<FaceId>
    generate_path(
        const Node & node
    ) {
        std::vector<FaceId> path;
        const Node * iter = &node;
        path.push_back(iter->m_id.m_next_face_id);
        do {
            path.push_back(iter->m_id.m_prev_face_id);
            iter = iter->m_state.m_parent_node;
        } while (iter != nullptr);
        return path;
    }
};

} // namespace nav2
