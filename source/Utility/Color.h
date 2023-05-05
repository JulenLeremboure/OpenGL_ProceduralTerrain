#pragma once

#include "Matrix.h"

template<typename T>
struct Color
{
    Color(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0, const T& a_ = 1) : r(r_), g(g_), b(b_), a(a_) {}
    Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

    T r = 0;
    T g = 0;
    T b = 0;
    T a = 1;

	Color convertRGBto01()
    {
        return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
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

using ColorF = Color<float>;

namespace GameColors
{
	inline ColorF sky           { ColorF(178, 194, 225).convertRGBto01() };
	inline ColorF abyssWater    { ColorF(11, 36, 71).convertRGBto01() };
	inline ColorF deepwater     { ColorF(25, 55, 109).convertRGBto01() };
	inline ColorF water         { ColorF(130, 148, 196).convertRGBto01() };
	inline ColorF cley          { ColorF(214, 205, 164).convertRGBto01() };
	inline ColorF sand          { ColorF(247, 208, 96).convertRGBto01() };
	inline ColorF grass         { ColorF(61, 131, 97).convertRGBto01() };
	inline ColorF rock          { ColorF(130, 125, 120).convertRGBto01() };
	inline ColorF hardrock      { ColorF(80, 75, 70).convertRGBto01() };
	inline ColorF snow          { ColorF(236, 242, 255).convertRGBto01() };
	inline ColorF ice           { ColorF(165, 215, 232).convertRGBto01() };
}

namespace BiomeColors
{
    inline ColorF ocean{ ColorF(10, 77, 104).convertRGBto01() };
    inline ColorF beach{ ColorF(235, 216, 178).convertRGBto01() };
    inline ColorF scorch{ ColorF(255, 123, 84).convertRGBto01() };
    inline ColorF bare{ ColorF(156, 167, 119).convertRGBto01() };
    inline ColorF tundra{ ColorF(169, 175, 126).convertRGBto01() };
    inline ColorF snow{ ColorF(236, 242, 255).convertRGBto01() };
    inline ColorF temparatedDesert{ ColorF(254, 168, 47).convertRGBto01() };
    inline ColorF shrubland{ ColorF(164, 190, 123).convertRGBto01() };
    inline ColorF taiga{ ColorF(40, 84, 48).convertRGBto01() };
    inline ColorF grassland{ ColorF(95, 141, 78).convertRGBto01() };
    inline ColorF temparatedDeciduousForest{ ColorF(231, 177, 10).convertRGBto01() };
    inline ColorF temparatedRainForest{ ColorF(43, 122, 11).convertRGBto01() };
    inline ColorF subtropicalDesert{ ColorF(250, 214, 165).convertRGBto01() };
    inline ColorF tropicalSeasonalDesert{ ColorF(155, 161, 123).convertRGBto01() };
}