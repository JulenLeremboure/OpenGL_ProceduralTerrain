#include "HeightMap.h"

#include "../../Rendering/Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


HeightMap::HeightMap() : m_seed(10), m_octaves(3), m_frequency(0.01f), m_lacunarity(2.0f), 
						m_gain(0.5f), m_noiseType(FastNoiseLite::NoiseType_OpenSimplex2S),
						m_fractalType(FastNoiseLite::FractalType_DomainWarpIndependent), m_cellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq),
						m_pow(1.f)
{
	load();
}

HeightMap::~HeightMap()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

double HeightMap::noise(double x, double y) { 
	// Rescale from -1.0:+1.0 to 0.0:1.0
	return std::pow(m_noiseGen.GetNoise(x, y) / 2.0 + 0.5, m_pow);
}

double HeightMap::multipleNoise(double x, double y, double f) {
	// Rescale from -1.0:+1.0 to 0.0:1.0
	double coeff = 0.;
	float vertexHeight = 0.f;
	while (f > 0)
	{
		FastNoiseLite noiseGen;
		vertexHeight += 1 / f * noise(f * x, f * y);
		coeff += 1 / f;
		f--;
	}

	return vertexHeight / coeff;
}


void HeightMap::clear()
{
	// Nettoyage des données
	m_points.clear();

	// Nettoyage des buffers
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteVertexArrays(1, &m_vao);

	// Réinitialisation des variables
	m_vao = 0;
	m_vbo = 0;
	m_ebo = 0;
	m_program = 0;
}


void HeightMap::load()
{
	constexpr float MAP_POS_Y_OFFSET = 50;

	// todo move to a function
	m_noiseGen.SetFractalType(m_fractalType);
	m_noiseGen.SetFractalWeightedStrength(0.5f);
	m_noiseGen.SetFractalOctaves(m_octaves);
	m_noiseGen.SetFractalGain(m_gain);
	m_noiseGen.SetFrequency(m_frequency);
	m_noiseGen.SetFractalLacunarity(m_lacunarity);
	m_noiseGen.SetSeed(m_seed);
	m_noiseGen.SetNoiseType(m_noiseType);
	m_noiseGen.SetCellularDistanceFunction(m_cellularDistanceFunction);
	
	for (double y = 0; y < MAP_HEIGHT; y++)
	{
		for (double x = 0; x < MAP_WIDTH; x++)
		{
			float elevation = noise(x, y);

			// new vertex
			vertex_colored newVtColored;
			newVtColored.point = Point3Df(
				-MAP_HEIGHT / 2.0f + y,
				elevation * MAP_POS_Y_OFFSET * 2,
				-MAP_WIDTH / 2.0f + x);

			newVtColored.color = getColorFromVertexHeight(elevation);

			m_points.emplace_back(newVtColored);
		}
	}

	// index generation
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < MAP_HEIGHT - 1; i++) // for each row a.k.a. each strip
	{
		for (unsigned int j = 0; j < MAP_WIDTH; j++) // for each column
		{
			for (unsigned int k = 0; k < 2; k++) // for each side of the strip
			{
				indices.push_back(j + MAP_WIDTH * (i + k));
			}
		}
	}

	// register VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		m_points.size() * sizeof(vertex_colored),	// size of vertices buffer
		&m_points[0],								// pointer to first element
		GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),	// size of indices buffer
		&indices[0],							// pointer to first element
		GL_STATIC_DRAW);

	ShaderInfo shader[] = {
		{ GL_VERTEX_SHADER, "Resources/Shaders/Cube/cube.vert" },
		{ GL_FRAGMENT_SHADER, "Resources/Shaders/Cube/cube.frag" },
		{ GL_NONE, nullptr }
	};

	const auto program = Shader::loadShaders(shader);
	glUseProgram(program);
	m_program = program;

	// position attribute
	glVertexAttribPointer(
		0, // Index
		3, // Size
		GL_FLOAT, // Type
		GL_FALSE, // normalized
		sizeof(vertex_colored), // Stride
		static_cast<void*>(0)); // Pointer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_colored), (char*)(0) + sizeof(vertex_colored::point));
	glEnableVertexAttribArray(1);
}

void HeightMap::render(Camera& camera, const float aspect_ratio)
{
	constexpr int NUM_STRIPS = MAP_HEIGHT - 1;
	constexpr int NUM_VERTS_PER_STRIP = MAP_WIDTH * 2;

	// draw mesh
	glBindVertexArray(m_vao);

	// render the mesh triangle strip by triangle strip - each row at a time
	for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
	{
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			NUM_VERTS_PER_STRIP, // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned int)
				* NUM_VERTS_PER_STRIP
				* strip)); // offset to starting index
	}

	const auto mvpLocation = glGetUniformLocation(m_program, "model");

	const auto matView = glm::lookAt(camera.m_cameraPos, camera.m_cameraPos + camera.m_cameraFront, camera.m_cameraUp);
	const auto matProj = glm::perspective(CAMERA_FOV, aspect_ratio, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE);
	const auto matViewByProj = matProj * matView;

	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &matViewByProj[0][0]);
}

Color<float> HeightMap::getColorFromVertexHeight(float elevation)
{
	constexpr float ABYSSWATER_THRESHOLD = 0.03f;
	constexpr float DEEPWATER_THRESHOLD = 0.08f;
	constexpr float WATER_THRESHOLD = 0.18f;
	constexpr float CLAY_THRESHOLD = 0.22f;
	constexpr float SAND_THRESHOLD = 0.3f;
	constexpr float GRASS_THRESHOLD = 0.6f;
	constexpr float ROCK_THRESHOLD = 0.75f;
	constexpr float HARDROCK_THRESHOLD = 0.85f;
	constexpr float SNOW_THRESHOLD = 0.95f;

	if (elevation < ABYSSWATER_THRESHOLD)
		return GameColors::abyssWater;

	if (elevation < DEEPWATER_THRESHOLD)
		return GameColors::deepwater;

	if (elevation < WATER_THRESHOLD)
		return GameColors::water;

	if (elevation < CLAY_THRESHOLD)
		return GameColors::cley;

	if (elevation < SAND_THRESHOLD)
		return GameColors::sand;

	if (elevation < GRASS_THRESHOLD)
		return GameColors::grass;

	if (elevation < ROCK_THRESHOLD)
		return GameColors::rock;

	if (elevation < HARDROCK_THRESHOLD)
		return GameColors::hardrock;

	if (elevation < SNOW_THRESHOLD)
		return GameColors::snow;

	return GameColors::ice;
}
