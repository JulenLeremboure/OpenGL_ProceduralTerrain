#pragma once

#include <vector>

#include "../../Utility/Color.h"
#include "gl/glew.h"

constexpr int MAP_HEIGHT = 100;
constexpr int MAP_WIDTH = 100;

constexpr int MIN_VERTEX_HEIGHT = 0;
constexpr int MAX_VERTEX_HEIGHT = 20;

class HeightMap
{
	using vertex_colored = vertex_struct_color<float>;

public:
	HeightMap();
	~HeightMap();

	void load();
	void render(const Mat4<float> viewProjection);

private:
	static Color<float> getColorFromVertexHeight(float vertexHeight);

	GLuint m_vao { 0 };
	GLuint m_vbo{ 0 };
	GLuint m_ebo{ 0 };
	GLuint m_program{ 0 };

	std::vector<vertex_colored> m_points;
};