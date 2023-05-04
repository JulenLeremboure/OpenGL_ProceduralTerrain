#pragma once

#include <vector>
#include "../../Noise/Noise.h"
#include "../../Utility/Color.h"
#include "gl/glew.h"
#include "../../SceneObjects/Camera/Camera.h"

constexpr int MAP_HEIGHT = 1000;
constexpr int MAP_WIDTH = 1000;

class HeightMap
{
	using vertex_colored = vertex_struct_color<float>;

public:
	HeightMap();
	~HeightMap();

	void load();
	void clear();
	void render(Camera& camera, const float aspect_ratio);
	double noise(double x, double y);
	double noise(FastNoiseLite noiseGen, double x, double y);
	void test();

	// todo make a struct noise_params
	int m_seed;
	float m_frequency;
	int m_octaves;
	float m_lacunarity;
	float m_gain;
	FastNoiseLite::NoiseType m_noiseType;

private:
	static Color<float> getColorFromVertexHeight(float elevation);

	GLuint m_vao { 0 };
	GLuint m_vbo{ 0 };
	GLuint m_ebo{ 0 };
	GLuint m_program{ 0 };

	// Create and configure FastNoise object
	FastNoiseLite m_noiseGen;
	
	/*float mFractalBounding;*/
	/*CellularDistanceFunction mCellularDistanceFunction;
	CellularReturnType mCellularReturnType;
	float mCellularJitterModifier;*/

	/*DomainWarpType mDomainWarpType;
	TransformType3D mWarpTransformType3D;
	float mDomainWarpAmp;*/
	

	std::vector<vertex_colored> m_points;
};