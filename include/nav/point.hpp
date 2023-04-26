#pragma once
#ifndef NAV__POINT__HPP
#define NAV__POINT__HPP

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
};

#endif