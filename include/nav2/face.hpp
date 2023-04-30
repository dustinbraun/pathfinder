#pragma once

#include <cstdint>

#include "face_id.hpp"
#include "vertex_id.hpp"

namespace nav2 {

class Face {
public:
    VertexId m_vertex_ids[3];

    FaceId m_adj_ids[3];


    bool
    contains_vertex_id(
        VertexId vertex_id
    ) const {
        return (m_vertex_ids[0] == vertex_id) || (m_vertex_ids[1] == vertex_id) || (m_vertex_ids[2] == vertex_id);
    }

    bool
    contains_adj_id(
        FaceId adj_id
    ) const {
        return (m_adj_ids[0] == adj_id) || (m_adj_ids[1] == adj_id) || (m_adj_ids[2] == adj_id);
    }
    
    void
    get_edge_vertex_ids(
        size_t edge_index,
        VertexId & vertex_id_a,
        VertexId & vertex_id_b
    ) const {
        assert(edge_index <= 2);
        switch (edge_index) {
            case 0: vertex_id_a = m_vertex_ids[0]; vertex_id_b = m_vertex_ids[1]; break;
            case 1: vertex_id_a = m_vertex_ids[1]; vertex_id_b = m_vertex_ids[2]; break;
            case 2: vertex_id_a = m_vertex_ids[2]; vertex_id_b = m_vertex_ids[0]; break;
        }
    }

    bool
    get_shared_edge_index(
        const Face & face_b,
        size_t & edge_index
    ) const {
        for (size_t i = 0; i < 3; ++i) {
            VertexId vertex_id_a;
            VertexId vertex_id_b;
            get_edge_vertex_ids(i, vertex_id_a, vertex_id_b);
            if (face_b.contains_vertex_id(vertex_id_a) && face_b.contains_vertex_id(vertex_id_b)) {
                edge_index = i;
                return true;
            }
        }
        return false;
    }

    size_t
    get_edge_index_by_adj_id(
        FaceId adj_id
    ) const {
        if (m_adj_ids[0] == adj_id) {
            return 0;
        }
        if (m_adj_ids[1] == adj_id) {
            return 1;
        }
        if (m_adj_ids[2] == adj_id) {
            return 2;
        }
        assert(false);
    }
};

} // namespace nav2
