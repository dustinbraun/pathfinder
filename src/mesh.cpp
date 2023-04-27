#include "nav/mesh.hpp"

bool
Mesh::point_is_inside_face(
    const Face & face,
    const Point & point
) const {
    const Vertex & vertex_a = m_vertices[face.m_vertex_ids[0]];
    const Vertex & vertex_b = m_vertices[face.m_vertex_ids[1]];
    const Vertex & vertex_c = m_vertices[face.m_vertex_ids[2]];

    if (same_side(point, vertex_a.m_pos, vertex_b.m_pos, vertex_c.m_pos) &&
        same_side(point, vertex_b.m_pos, vertex_a.m_pos, vertex_c.m_pos) &&
        same_side(point, vertex_c.m_pos, vertex_a.m_pos, vertex_b.m_pos)) {

        return true;
    }
    
    return false;
}

FaceId
Mesh::get_face_id_containing_point(
    const Point & point
) const {
    for (FaceId face_id = 0; face_id < m_faces.size(); ++face_id) {
        const Face & face = m_faces[face_id];
        if (point_is_inside_face(face, point)) {
            return face_id;
        }
    }
    return FACE_ID_NONE;
}

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

VertexId
Mesh::get_vertex_id_near_point(
    const Point & point,
    float max_distance
) const {
    for (VertexId vertex_id = 0; vertex_id < m_vertices.size(); vertex_id++) {
        if (m_vertices[vertex_id].m_pos.get_square_distance(point) <= max_distance*max_distance) {
            return vertex_id;
        }
    }
    return VERTEX_ID_NONE;
}

VertexId
Mesh::get_nearest_vertex_id_to_point(
    const Point & point
) const {
    if (m_vertices.empty()) {
        return VERTEX_ID_NONE;
    }
    VertexId nearest_vertex_id = 0;
    float nearest_vertex_square_distance_to_point = m_vertices[nearest_vertex_id].m_pos.get_square_distance(point);
    for (VertexId vertex_id = 1; vertex_id < m_vertices.size(); ++vertex_id) {
        float square_distance_to_point = m_vertices[vertex_id].m_pos.get_square_distance(point);
        if (square_distance_to_point < nearest_vertex_square_distance_to_point) {
            nearest_vertex_id = vertex_id;
            nearest_vertex_square_distance_to_point = square_distance_to_point;
        }
    }
    return nearest_vertex_id;
}

bool
Mesh::try_connect_face(
    FaceId face_id_a,
    FaceId face_id_b
) {
    Face & face_a = get_face_by_id(face_id_a);
    Face & face_b = get_face_by_id(face_id_b);
    size_t edge_index;
    if (face_a.get_shared_edge_index(face_b, edge_index)) {
        face_a.m_adj_ids[edge_index] = face_id_b;
        return true;
    }
    return false;
}

void
Mesh::connect(
) {
    for (size_t face_a_id = 0; face_a_id < m_faces.size(); ++face_a_id) {
        for (size_t face_b_id = face_a_id + 1; face_b_id < m_faces.size(); ++face_b_id) {
            try_connect_face(face_a_id, face_b_id);
            try_connect_face(face_b_id, face_a_id);
        }
    }
}


FaceId
Mesh::add_face(
    VertexId vertex_id0,
    VertexId vertex_id1,
    VertexId vertex_id2
) {
    Face face {
        .m_vertex_ids = {
            vertex_id0,
            vertex_id1,
            vertex_id2,
        },
        .m_adj_ids = {
            FACE_ID_NONE,
            FACE_ID_NONE,
            FACE_ID_NONE,
        },
    };
    assert(m_faces.size() <= FACE_ID_MAX);
    FaceId face_id = static_cast<FaceId>(m_faces.size());
    m_faces.push_back(face);
    return face_id;
}

VertexId
Mesh::add_vertex(
    float x,
    float y
) {
    Vertex vertex;
    vertex.m_pos.m_x = x;
    vertex.m_pos.m_y = y;
    assert(m_vertices.size() < VERTEX_ID_MAX);
    VertexId vertex_id = static_cast<VertexId>(m_vertices.size());
    m_vertices.push_back(vertex);
    return vertex_id;
}

void
Mesh::save(
    const char * path 
) const {
    std::ofstream f(path, std::ios::binary);
    f.clear();
    VertexId num_vertices = m_vertices.size();
    f.write(reinterpret_cast<const char *>(&num_vertices), sizeof(VertexId));
    for (auto vertex : m_vertices) {
        f.write(reinterpret_cast<const char *>(&vertex), sizeof(Vertex));
    }
    FaceId num_faces = m_faces.size();
    f.write(reinterpret_cast<const char *>(&num_faces), sizeof(FaceId));
    for (auto face : m_faces) {
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[0]), sizeof(VertexId));
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[1]), sizeof(VertexId));
        f.write(reinterpret_cast<const char *>(&face.m_vertex_ids[2]), sizeof(VertexId));
    }
}

void
Mesh::load(
    const char * path 
) {
    m_vertices.clear();
    m_faces.clear();
    std::ifstream f(path, std::ios::binary);
    VertexId num_vertices;
    f.read(reinterpret_cast<char *>(&num_vertices), sizeof(VertexId));
    std::cout << num_vertices << std::endl;
    m_vertices.resize(num_vertices);
    for (VertexId vertex_id = 0; vertex_id < num_vertices; ++vertex_id) {
        f.read(reinterpret_cast<char *>(&m_vertices.data()[vertex_id]), sizeof(Vertex));
    }
    FaceId num_faces;
    f.read(reinterpret_cast<char *>(&num_faces), sizeof(FaceId));
    std::cout << num_faces << std::endl;
    m_faces.resize(num_faces);
    for (auto & face : m_faces) {
        face.m_adj_ids[0] = FACE_ID_NONE;
        face.m_adj_ids[1] = FACE_ID_NONE;
        face.m_adj_ids[2] = FACE_ID_NONE;
    }
    for (FaceId face_id = 0; face_id < num_faces; ++face_id) {
        f.read(reinterpret_cast<char *>(&m_faces.data()[face_id].m_vertex_ids[0]), sizeof(VertexId));
        f.read(reinterpret_cast<char *>(&m_faces.data()[face_id].m_vertex_ids[1]), sizeof(VertexId));
        f.read(reinterpret_cast<char *>(&m_faces.data()[face_id].m_vertex_ids[2]), sizeof(VertexId));
    }
    connect();
}

