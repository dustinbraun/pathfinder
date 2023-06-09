#pragma once

#include <cassert>
#include <iostream>
#include <vector>

#include "edge_location.hpp"
#include "face.hpp"
#include "face_data.hpp"
#include "point.hpp"
#include "vertex.hpp"
#include "vertex_data.hpp"

namespace nav2 {

class Mesh {
public:
    Point
    get_point_of_edge_location(
        const Face & face,
        size_t edge_index,
        EdgeLocation edge_location
    ) const {
        VertexId vertex_id_a;
        VertexId vertex_id_b;
        face.get_edge_vertex_ids(edge_index, vertex_id_a, vertex_id_b);
        const Vertex & vertex_a = m_vertices[vertex_id_a];
        const Vertex & vertex_b = m_vertices[vertex_id_b];
        if (edge_location == EdgeLocation::MIDDLE) {
            return Point(
                (vertex_a.m_pos.m_x + vertex_b.m_pos.m_x) / 2.0f,
                (vertex_a.m_pos.m_y + vertex_b.m_pos.m_y) / 2.0f
            );
        }
        if (edge_location == EdgeLocation::LEFT) {
            // Assuming faces are in CCW order.
            return vertex_b.m_pos;
        }
        if (edge_location == EdgeLocation::RIGHT) {
            // Assuming faces are in CCW order.
            return vertex_a.m_pos;
        }
        assert(false);
    }

    void
    import(
        const VertexData * vertices,
        size_t num_vertices,
        const FaceData * faces,
        size_t num_faces
    ) {
        assert(num_vertices <= UINT16_MAX);
        m_vertices.clear();
        m_vertices.reserve(num_vertices);
        for (size_t vertex_id = 0; vertex_id < num_vertices; ++vertex_id) {
            const VertexData & vertex_data = vertices[vertex_id];
            Vertex vertex;
            vertex.m_pos.m_x = vertex_data.m_x;
            vertex.m_pos.m_y = vertex_data.m_y;
            m_vertices.push_back(vertex);
        }
        assert(num_vertices <= UINT16_MAX);
        m_faces.clear();
        m_faces.reserve(num_faces);
        for (size_t face_id = 0; face_id < num_faces; ++face_id) {
            const FaceData & face_data = faces[face_id];
            Face face;
            face.m_vertex_ids[0] = face_data.m_vertex_ids[0];
            face.m_vertex_ids[1] = face_data.m_vertex_ids[1];
            face.m_vertex_ids[2] = face_data.m_vertex_ids[2];
            face.m_adj_ids[0] = FACE_ID_NONE;
            face.m_adj_ids[1] = FACE_ID_NONE;
            face.m_adj_ids[2] = FACE_ID_NONE;
            m_faces.push_back(face);
        }
        connect_faces();
    }

    // For unit-test/bench data generation
    void
    export_cpp() {
        std::cout << "uint16_t vertex_count = " << m_vertices.size() << ";" << std::endl;
        std::cout << "VertexData vertices[] = {" << std::endl;
        for (auto vertex : m_vertices) {
            std::cout << "    { " << vertex.m_pos.m_x << ", " << vertex.m_pos.m_y << ", }," << std::endl;
        }
        std::cout << "};" << std::endl;

        std::cout << "uint16_t face_count = " << m_faces.size() << ";" << std::endl;
        std::cout << "FaceData faces[] = {" << std::endl;
        for (auto face : m_faces) {
            std::cout << "    { " << face.m_vertex_ids[0] << ", " << face.m_vertex_ids[1] << ", " << face.m_vertex_ids[2] << ", }," << std::endl;
        }
        std::cout << "};" << std::endl;
    }

    bool
    point_is_inside_face(
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
    get_face_id_containing_point(
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
    get_face_center_point(
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
    get_edge_center_point(
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
    get_vertex_id_near_point(
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
    get_nearest_vertex_id_to_point(
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

    Vertex &
    get_vertex_by_id(
        VertexId vertex_id
    ) {
        assert(static_cast<size_t>(vertex_id) < m_vertices.size());
        return m_vertices[vertex_id];
    }

    const Vertex &
    get_vertex_by_id(
        VertexId vertex_id
    ) const {
        assert(static_cast<size_t>(vertex_id) < m_vertices.size());
        return m_vertices[vertex_id];
    }

    Face &
    get_face_by_id(
        FaceId face_id 
    ) {
        assert(static_cast<size_t>(face_id) < m_faces.size());
        return m_faces[face_id];
    }

    const Face &
    get_face_by_id(
        FaceId face_id 
    ) const {
        assert(static_cast<size_t>(face_id) < m_faces.size());
        return m_faces[face_id];
    }

    uint16_t
    get_num_vertices() const {
        return m_vertices.size();
    }

    uint16_t
    get_num_faces() const {
        return m_faces.size();
    }

private:
    std::vector<Face> m_faces;

    std::vector<Vertex> m_vertices;

    
    bool
    try_connect_face(
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
    connect_faces(
    ) {
        for (size_t face_a_id = 0; face_a_id < m_faces.size(); ++face_a_id) {
            for (size_t face_b_id = face_a_id + 1; face_b_id < m_faces.size(); ++face_b_id) {
                try_connect_face(face_a_id, face_b_id);
                try_connect_face(face_b_id, face_a_id);
            }
        }
    }

    static
    float cross(
        Point a,
        Point b 
    ) {
        return a.m_x*b.m_y - a.m_y*b.m_x;
    }

    static
    bool same_side(
        Point p1,
        Point p2,
        Point a,
        Point b
    ) {
        float cp1 = cross(b - a, p1 - a);
        float cp2 = cross(b - a, p2 - a);
        if (cp1*cp2 >= 0.0f) {
            return true;
        }
        return false;
    }

};

} // namespace nav2