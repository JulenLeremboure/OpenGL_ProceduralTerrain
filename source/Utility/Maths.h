#pragma once

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG_TO_RAD = PI * 180.f;
constexpr float RAD_TO_DEG = 1.f / DEG_TO_RAD;
constexpr float EPSILON = 0.00001f;

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

    Point3D<T> operator+(const Point3D<T> other) { return { x + other.x, y + other.y , z + other.z }; }
    Point3D<T> operator-(const Point3D<T> other) { return { x - other.x, y - other.y , z - other.z }; }
    Point3D<T> operator*(const Point3D<T> other) { return { x * other.x, y * other.y , z * other.z }; }
    Point3D<T> operator*(const T& val) { return { x * val, y * val, z * val }; }
    Point3D<T> operator/(const Point3D<T> other) { return { x / other.x, y / other.y , z / other.z }; }
    Point3D<T> operator/(const T& val) { return { x / val, y / val, z / val }; }
    Point3D<T> operator-() { return { *this * -1 }; }

    friend bool operator==(const Point3D<T>& lhs, const Point3D<T>& rhs)
    {
        Point3D<T> diffPoint3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);

        // To avoid direct float comparison, checking with EPSILON
        return diffPoint3D.SqrMagnitude() < EPSILON * EPSILON;
    }

    float Dot(const Point3D<T> other) { return x * other.x + y * other.y + z * other.z; }
    float Magnitude() { return std::sqrt(Dot(*this)); }
    float SqrMagnitude() { return Dot(*this); }

    Point3D<T> Normalized() { return *this / Magnitude(); }
};

class MathUtils
{
public:
    static float remapValue(float value, float fromBoundsStart, float fromBoundsEnd, float toBoundsStart, float toBoundsEnd)
	{
        return (value - fromBoundsStart) / (fromBoundsEnd - fromBoundsStart) * (toBoundsEnd - toBoundsStart) + toBoundsStart;
    }
};

using Point3Df = Point3D<float>;
using Point2Df = Point2D<float>;