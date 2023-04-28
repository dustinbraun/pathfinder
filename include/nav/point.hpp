#pragma once
#ifndef NAV__POINT__HPP
#define NAV__POINT__HPP

#include <cmath>

class Point {
public:
    float m_x;
    float m_y;


    Point(
    ) {
        
    }

    Point(
        float x,
        float y
    ) : m_x(x), m_y(y) {

    }

    Point
    operator - (
        const Point & rhs
    ) const {
        return Point(
            m_x - rhs.m_x,
            m_y - rhs.m_y
        );
    }

    float
    get_square_distance(
        const Point& other
    ) const {
        float dx = m_x - other.m_x;
        float dy = m_y - other.m_y;
        return dx*dx + dy*dy;
    }

    float
    get_distance(
        const Point & other
    ) const {
        return sqrtf(get_square_distance(other));
    }
};

#endif