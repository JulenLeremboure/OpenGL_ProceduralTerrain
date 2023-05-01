#pragma once

#include "Utility/Matrix.h"

template<typename T>
struct Color
{
    Color(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0, const T& a_ = 1) : r(r_), g(g_), b(b_), a(a_) {}
    Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

    T r = 0;
    T g = 0;
    T b = 0;
    T a = 1;

	static Color convertRGBto01(const T& r_, const T& g_, const T& b_, const T& a_ = 255)
    {
        return { r_ / 255.f, g_ / 255.f, b_ / 255.f, a_ / 255.f };
    }
};

template<typename T>
struct vertex_struct_color
{
    vertex_struct_color(const Point3D<T>& p = Point3D<T>(), const Color<T>& c = Color<T>())
        : point(p)
        , color(c)
    {}

    Point3D<T> point;
    Color<T> color;
};
