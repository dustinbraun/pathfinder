#pragma once

#include <unordered_map>
#include <vector>

#include "mesh.hpp"

class NodeEx {
public:
    FaceId m_next_face_id;
    FaceId m_prev_face_id;
    Point m_position;
    uint16_t m_is_enqueued_salt;
    uint16_t m_is_visited_salt;
    const NodeEx * m_parent_node;
    float m_f;
    float m_g;
};

class NodeMapping {
public:
    uint16_t m_node_indices[3];
};

uint32_t get_key(uint16_t face_id_a, uint16_t face_id_b) {
    if (face_id_a < face_id_b) {
        return static_cast<uint32_t>(face_id_a) | (static_cast<uint32_t>(face_id_b) << 16);
    }
    else {
        return static_cast<uint32_t>(face_id_b) | (static_cast<uint32_t>(face_id_a) << 16);
    }
}

class NodeArenaEx {
public:
    void clear() {
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
        const NodeEx & node
    ) const {
        return node.m_is_enqueued_salt == m_salt;
    }

    bool
    is_visited(
        const NodeEx & node
    ) const {
        return node.m_is_visited_salt == m_salt;
    }

    void
    mark_as_enqueued(
        NodeEx & node
    ) {
        node.m_is_enqueued_salt = m_salt;
    }

    void
    mark_as_visited(
        NodeEx & node
    ) {
        node.m_is_visited_salt = m_salt;
    }

    void
    mark_as_dequeued(
        NodeEx & node
    ) {
        node.m_is_enqueued_salt = 0;
    }

    NodeArenaEx(
        const Mesh& mesh
    ) : m_node_mapping(new NodeMapping[mesh.get_num_faces()]), m_num_mappings(mesh.get_num_faces()), m_salt(1) {
        clear();
        for (size_t face_id = 0; face_id < mesh.get_num_faces(); ++face_id) {
            m_node_mapping[face_id].m_node_indices[0] = FACE_ID_NONE;
            m_node_mapping[face_id].m_node_indices[1] = FACE_ID_NONE;
            m_node_mapping[face_id].m_node_indices[2] = FACE_ID_NONE;
        }
        std::unordered_map<uint32_t, uint16_t> map(mesh.get_num_faces()*2);
        for (size_t face_id = 0; face_id < mesh.get_num_faces(); ++face_id) {
            const Face & face = mesh.get_face_by_id(face_id);
            for (size_t edge_index = 0; edge_index < 3; ++edge_index) {
                FaceId adj_id = face.m_adj_ids[edge_index];
                if (adj_id != FACE_ID_NONE) {
                    auto map_iter = map.find(get_key(face_id, adj_id));
                    if (map_iter == map.end()) {
                        m_node_mapping[face_id].m_node_indices[edge_index] = m_nodes.size();
                        map[get_key(face_id, adj_id)] = m_nodes.size();
                        NodeEx node;
                        m_nodes.push_back(node);
                    }
                    else {
                        m_node_mapping[face_id].m_node_indices[edge_index] = map_iter->second;
                    }
                }
            }
        }
    }

    NodeEx & get_node(FaceId face_id, size_t edge_index) {
        size_t node_index = m_node_mapping[face_id].m_node_indices[edge_index];
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
            std::cout << m_node_mapping[i].m_node_indices[0] << std::endl;
            std::cout << m_node_mapping[i].m_node_indices[1] << std::endl;
            std::cout << m_node_mapping[i].m_node_indices[2] << std::endl;
        }
    }

private:
    std::vector<NodeEx> m_nodes;

    NodeMapping * m_node_mapping;

    size_t m_num_mappings;

    uint16_t m_salt;
};
