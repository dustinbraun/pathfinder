#pragma once

#include <array>
#include <vector>

#include "mesh.hpp"

class NodeEx {
    friend class NodeArenaEx;

public:
    FaceId m_next_face_id;
    FaceId m_prev_face_id;
    
    const Point m_position;
    
    const NodeEx *m_parent_node;
    
    float m_f;
    float m_g;


    NodeEx(
        const Point & position
    ) : m_position(position) {

    }

private:
    uint16_t m_is_enqueued_salt;
    uint16_t m_is_visited_salt;
};

// This NodeArena does not use a HashMap to store and map nodes.
// Instead, a direct mapping between Face edges and the nodes are pre-computed
// during construction.
// Nodes have an indication whether they are enqueued in the open-list or not.
// This NodeArena does not use a boolean flag, instead, it uses a salted value
// for each node which is compared with the salt value of the current search
// (the salt is increased every search iteration).
// This way it is not necessary to iterate each node for each search to reset
// the flags. This is beneficial especially when the NodeArena becomes very big.
class NodeArenaEx {
public:
    NodeArenaEx(
        const Mesh &mesh
    ) : m_node_mapping(new std::array<uint16_t, 3>[mesh.get_num_faces()]), m_num_mappings(mesh.get_num_faces()), m_salt(1) {
        for (auto & node : m_nodes) {
            node.m_is_visited_salt = 0;
            node.m_is_enqueued_salt = 0;
        }
        for (size_t face_id = 0; face_id < mesh.get_num_faces(); ++face_id) {
            const Face &face = mesh.get_face_by_id(face_id);
            for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
                FaceId adj_id = face.m_adj_ids[edge_index];
                if (adj_id == FACE_ID_NONE) {
                    // There is no adjacent face on this edge, so there is no mapping.
                    m_node_mapping[face_id][edge_index] = FACE_ID_NONE;
                }
                else {
                    if (adj_id < face_id) {
                        // The adjacent face has a lower id than the current face, this
                        // means the edge node was already added during a previous loop iteration.
                        // We take the node index from the entry of the adjacent face so that
                        // both faces point to the same node.
                        // Remember, faces have the same node on shared edges.
                        m_node_mapping[face_id][edge_index] = m_node_mapping[adj_id][mesh.get_face_by_id(adj_id).get_edge_index_by_adj_id(face_id)];
                    }
                    else {
                        // The current face has a lower id than the adjacent face, this
                        // means the edge node does not exist yet.
                        m_node_mapping[face_id][edge_index] = static_cast<uint16_t>(m_nodes.size());
                        m_nodes.emplace_back(mesh.get_edge_center_point(face, edge_index));
                    }
                }
            }
        }
    }

    ~NodeArenaEx() {
        delete[] m_node_mapping;
    }

    void
    clear() {
        if (m_salt == UINT16_MAX) {
            for (auto & node : m_nodes) {
                node.m_is_visited_salt = 0;
                node.m_is_enqueued_salt = 0;
            }
            m_salt = 1;
        }
        else {
            m_salt++;
        }
    }

    bool
    is_enqueued(
        const NodeEx &node
    ) const {
        return node.m_is_enqueued_salt == m_salt;
    }

    bool
    is_visited(
        const NodeEx &node
    ) const {
        return node.m_is_visited_salt == m_salt;
    }

    void
    mark_as_enqueued(
        NodeEx &node
    ) {
        node.m_is_enqueued_salt = m_salt;
    }

    void
    mark_as_visited(
        NodeEx &node
    ) {
        node.m_is_visited_salt = m_salt;
    }

    void
    mark_as_dequeued(
        NodeEx &node
    ) {
        node.m_is_enqueued_salt = 0;
    }

    NodeEx &
    get_node(
        FaceId face_id,
        size_t edge_index
    ) {
        size_t node_index = m_node_mapping[face_id][edge_index];
        assert(node_index != 0xFFFF);
        return m_nodes[node_index];
    }

    const NodeEx *
    get_nodes() const {
        return m_nodes.data();
    }

    size_t
    get_num_nodes() const {
        return m_nodes.size();
    }

    void
    print() {
        for (size_t i = 0; i < m_num_mappings; ++i) {
            std::cout << m_node_mapping[i][0] << std::endl;
            std::cout << m_node_mapping[i][1] << std::endl;
            std::cout << m_node_mapping[i][2] << std::endl;
        }
    }

private:
    std::vector<NodeEx> m_nodes;

    std::array<uint16_t, 3> * m_node_mapping;

    size_t m_num_mappings;

    uint16_t m_salt;
};
