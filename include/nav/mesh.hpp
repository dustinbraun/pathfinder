#pragma once
#ifndef NAV__MESH__HPP
#define NAV__MESH__HPP

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "face.hpp"
#include "face_data.hpp"
#include "point.hpp"
#include "vertex.hpp"
#include "vertex_data.hpp"

class Mesh {
public:
    void
    import(
        const VertexData * vertices,
        size_t num_vertices,
        const FaceData * faces,
        size_t num_faces
    );

    bool
    point_is_inside_face(
        const Face & face,
        const Point & point
    ) const;

    FaceId
    get_face_id_containing_point(
        const Point & point
    ) const;

    VertexId
    get_vertex_id_near_point(
        const Point & point,
        float max_distance
    ) const;

    VertexId
    get_nearest_vertex_id_to_point(
        const Point & point
    ) const;

    size_t
    get_num_faces(
    ) const {
        return m_faces.size();
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

    Point
    get_face_center_point(
        const Face & face
    ) const;

    Point
    get_edge_center_point(
        const Face & face,
        size_t edge_index
    ) const;

private:
    std::vector<Face> m_faces;

    std::vector<Vertex> m_vertices;

    bool
    try_connect_face(
        FaceId face_id_a,
        FaceId face_id_b
    );

    void
    connect_faces();

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

#endif