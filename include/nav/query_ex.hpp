#pragma once

#include "mesh.hpp"
#include "node_arena_ex.hpp"
#include "node_queue_ex.hpp"

class QueryEx {
public:
    QueryEx(const Mesh & mesh, std::function<void(const Mesh &, const NodeArenaEx &)> debug_cb) : m_mesh(mesh), m_node_arena(mesh), m_node_queue(1024), m_debug_cb(debug_cb) {

    }

    std::vector<FaceId>
    search(FaceId start_face_id, Point start_position, FaceId end_face_id, Point end_position) {
        m_node_arena.clear();
        m_node_queue.clear();
        if (start_face_id == end_face_id) {
            std::vector<FaceId> path;
            path.push_back(end_face_id);
            return path;
            
        }
        const Face & start_face = m_mesh.get_face_by_id(start_face_id);
        for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
            FaceId next_face_id = start_face.m_adj_ids[edge_index];
            if (next_face_id != FACE_ID_NONE) {
                NodeEx & node = m_node_arena.get_node(start_face_id, edge_index);
                node.m_prev_face_id = start_face_id;
                node.m_next_face_id = next_face_id;
                node.m_g = start_position.get_distance(node.m_position);
                node.m_f = node.m_g + node.m_position.get_distance(end_position);
                node.m_parent_node = nullptr;
                m_node_arena.mark_as_enqueued(node);
                m_node_queue.insert_node(&node);
            }
        }
        while (!m_node_queue.is_empty()) {
            NodeEx & node = m_node_queue.pop_node();
            
            // DEBUG
            m_debug_cb(m_mesh, m_node_arena);

            if (node.m_next_face_id == end_face_id) {
                return generate_path(node);
            }
            m_node_arena.mark_as_dequeued(node);
            m_node_arena.mark_as_visited(node);
            expand_node(node, end_position);
        }
        return std::vector<FaceId>();
    }

private:
    const Mesh & m_mesh;

    NodeArenaEx m_node_arena;
    NodeQueueEx m_node_queue;

    std::function<void(const Mesh &, const NodeArenaEx &)> m_debug_cb;


    std::vector<FaceId>
    generate_path(
        const NodeEx & node
    ) {
        std::vector<FaceId> path;
        const NodeEx * iter = &node;
        path.push_back(iter->m_next_face_id);
        do {
            path.push_back(iter->m_prev_face_id);
            iter = iter->m_parent_node;
        } while (iter != nullptr);
        return path;
    }

    void
    expand_node(
        const NodeEx & node,
        const Point & end_position
    ) {
        for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
            FaceId prev_face_id = node.m_next_face_id;
            FaceId next_face_id = m_mesh.get_face_by_id(prev_face_id).m_adj_ids[edge_index];
            if (next_face_id != FACE_ID_NONE) {
                NodeEx & next_node = m_node_arena.get_node(prev_face_id, edge_index);
                if (!m_node_arena.is_visited(next_node)) {
                    if (m_node_arena.is_enqueued(next_node)) {
                        float g = node.m_g + node.m_position.get_distance(next_node.m_position);
                        if (g < next_node.m_g) {
                            // next_node.m_prev_face_id = prev_face_id;
                            // next_node.m_next_face_id = next_face_id;
                            next_node.m_g = g;
                            next_node.m_f = next_node.m_g + next_node.m_position.get_distance(end_position);
                            next_node.m_parent_node = &node;
                            m_node_queue.modify_node(next_node);
                        }
                    }
                    else {
                        // New node.
                        next_node.m_prev_face_id = prev_face_id;
                        next_node.m_next_face_id = next_face_id;
                        next_node.m_g = node.m_g + node.m_position.get_distance(next_node.m_position);
                        next_node.m_f = next_node.m_g + next_node.m_position.get_distance(end_position);
                        next_node.m_parent_node = &node;
                        m_node_arena.mark_as_enqueued(next_node);
                        m_node_queue.insert_node(&next_node);
                    }
                    // DEBUG
                    m_debug_cb(m_mesh, m_node_arena);
                }
            }
        }
    }
};