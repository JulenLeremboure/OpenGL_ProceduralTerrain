#pragma once

#include <vector>
#include "../../Noise/Noise.h"
#include "../../Utility/Color.h"
#include "gl/glew.h"

constexpr int MAP_HEIGHT = 1000;
constexpr int MAP_WIDTH = 1000;

constexpr int MIN_VERTEX_HEIGHT = 0;
constexpr int MAX_VERTEX_HEIGHT = 20;

class HeightMap
{
	using vertex_colored = vertex_struct_color<float>;

public:
	HeightMap();
	~HeightMap();

	void load(const int seed);
	void render(const glm::mat<4, 4, float>& viewProjection);
	double noise(double x, double y);
	double multipleNoise(double x, double y, double t);

private:
	static Color<float> getColorFromVertexHeight(float vertexHeight);

	GLuint m_vao { 0 };
	GLuint m_vbo{ 0 };
	GLuint m_ebo{ 0 };
	GLuint m_program{ 0 };

	// Create and configure FastNoise object
	FastNoiseLite noiseGen;

	std::vector<vertex_colored> m_points;
};