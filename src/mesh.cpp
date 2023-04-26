#include "nav/mesh.hpp"

Point
Mesh::get_face_center_point(
    const Face & face
) const {
        const Vertex & vertex_a = m_vertices[face.m_vertex_ids[0]];
        const Vertex & vertex_b = m_vertices[face.m_vertex_ids[1]];
        const Vertex & vertex_c = m_vertices[face.m_vertex_ids[2]];
        return Point(
            (vertex_a.m_pos.m_x + vertex_b.m_pos.m_x + vertex_c.m_pos.m_x) / 3.0f,
            (vertex_a.m_pos.m_y + vertex_b.m_pos.m_y + vertex_c.m_pos.m_y) / 3.0f
        );
}

Point
Mesh::get_edge_center_point(
    const Face & face,
    size_t edge_index
) const {
        VertexId vertex_id_a;
        VertexId vertex_id_b;
        face.get_edge_vertex_ids(edge_index, vertex_id_a, vertex_id_b);
        const Vertex & vertex_a = m_vertices[vertex_id_a];
        const Vertex & vertex_b = m_vertices[vertex_id_b];
        return Point(
            (vertex_a.m_pos.m_x + vertex_b.m_pos.m_x) / 2.0f,
            (vertex_a.m_pos.m_y + vertex_b.m_pos.m_y) / 2.0f
        );
}
