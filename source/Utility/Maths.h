#pragma once

constexpr float PI = 3.14159265358979323846f;

template<typename T>
struct Point2D
{
    Point2D(const T x = 0, const T y = 0) : x(x), y(y) {}
    Point2D(const Point2D& pt) : x(pt.x), y(pt.y) {}

    T x;
    T y;
};

template<typename T>
struct Point3D
{
    Point3D(const T x = 0, const T y = 0, const T z = 0) : x(x), y(y), z(z) {}
    Point3D(const Point3D& pt) : x(pt.x), y(pt.y), z(pt.z) {}

    T x;
    T y;
    T z;
};

using Point3Df = Point3D<float>;
using Point2Df = Point2D<float>;