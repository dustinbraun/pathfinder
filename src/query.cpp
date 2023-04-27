#include "nav/query.hpp"

std::vector<FaceId>
Query::search(
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
            for (size_t edge_location = 0; edge_location < 3; ++edge_location) {
                Node * node = m_node_arena.get_node(start_face_id, next_face_id, static_cast<EdgeLocation>(edge_location));
                if (node != nullptr) {
                    node->m_state.m_pos = mesh.get_point_of_edge_location(start_face, edge_index, static_cast<EdgeLocation>(edge_location)); //mesh.get_edge_center_point(start_face, edge_index);
                    node->m_state.m_g = compute_distance(start_pos, node->m_state.m_pos);
                    node->m_state.m_f = node->m_state.m_g + compute_distance(node->m_state.m_pos, end_pos);
                    node->m_state.m_is_open = true;
                    m_node_queue.insert_node(node);

                    // DEBUG
                    m_debug_cb(mesh, m_node_arena);
                }
            }
        }
    }
    while (!m_node_queue.is_empty()) {
        Node & node = m_node_queue.pop_node();
        if (node.m_next_face == end_face_id) {
            return generate_path(node);
        }
        node.m_state.m_is_open = false;
        node.m_state.m_is_closed = true;
        
        // DEBUG
        m_debug_cb(mesh, m_node_arena);
        expand_node(mesh, node, end_pos);
    }
    return std::vector<FaceId>();
}

void
Query::expand_node(
    const Mesh & mesh,
    Node & node,
    Point end_pos
) {
    for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
        FaceId next_face_id = mesh.get_face_by_id(node.m_next_face).m_adj_ids[edge_index];
        if (next_face_id != FACE_ID_NONE) {
            for (size_t edge_location = 0; edge_location < 3; ++edge_location) {
                Node * next_node = m_node_arena.get_node(node.m_next_face, next_face_id, static_cast<EdgeLocation>(edge_location));
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
                        next_node->m_state.m_pos = mesh.get_point_of_edge_location(mesh.get_face_by_id(node.m_next_face), edge_index, static_cast<EdgeLocation>(edge_location));
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
}

std::vector<FaceId>
Query::generate_path(
    const Node & node
) {
    std::vector<FaceId> path;
    const Node * iter = &node;
    path.push_back(iter->m_next_face);
    do {
        path.push_back(iter->m_prev_face);
        iter = iter->m_state.m_parent_node;
    } while (iter != nullptr);
    return path;
}