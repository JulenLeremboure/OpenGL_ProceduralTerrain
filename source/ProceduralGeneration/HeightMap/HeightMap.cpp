#include "HeightMap.h"

#include "../../Rendering/Shader.h"

HeightMap::HeightMap()
{
	load(rand());
}

HeightMap::~HeightMap()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

double HeightMap::noise(double x, double y) { 
	// Rescale from -1.0:+1.0 to 0.0:1.0
	return noiseGen.GetNoise(x, y) / 2.0 + 0.5;
}

double HeightMap::multipleNoise(double x, double y, double f) { 
	// Rescale from -1.0:+1.0 to 0.0:1.0
	//return noiseGen.GetNoise(x, y) / 2.0 + 0.5;
	double coeff = 0.;
	float vertexHeight = 0.f;
	while (f > 0)
	{
		vertexHeight += 1/f * noise(f * x, f * y);
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

void HeightMap::load(const int seed)
{
	constexpr float TERRAIN_WAVE_AMPLITUDE = MAX_VERTEX_HEIGHT / 2.f;
	constexpr float TERRAIN_WAVE_FREQUENCY = 1 / 8.f;
	constexpr float MAP_POS_Y_OFFSET = 50;

	noiseGen.SetSeed(seed);
	
	for (double y = 0; y < MAP_HEIGHT; y++)
	{
		for (double x = 0; x < MAP_WIDTH; x++)
		{
			//const float vertexHeight = std::sin(y * TERRAIN_WAVE_FREQUENCY) * TERRAIN_WAVE_AMPLITUDE + TERRAIN_WAVE_AMPLITUDE;
			//float vertexHeight = noise(x, y);
			float vertexHeight = multipleNoise(x, y, 4);

			// new vertex
			vertex_colored newVtColored;
			newVtColored.point = Point3Df(
				-MAP_HEIGHT / 2.0f + y,
				vertexHeight * MAP_POS_Y_OFFSET,
				-MAP_WIDTH / 2.0f + x);

			newVtColored.color = getColorFromVertexHeight(vertexHeight);

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

	glEnable(GL_DEPTH_TEST);
}

void HeightMap::render(const Mat4<float> viewProjection)
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
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, viewProjection.data());
}

Color<float> HeightMap::getColorFromVertexHeight(float vertexHeight)
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

	//vertexHeight = MathUtils::remapValue(vertexHeight, MIN_VERTEX_HEIGHT, MAX_VERTEX_HEIGHT, 0, 1);

	if (vertexHeight < ABYSSWATER_THRESHOLD)
		return GameColors::abyssWater;

	if (vertexHeight < DEEPWATER_THRESHOLD)
		return GameColors::deepwater;

	if (vertexHeight < WATER_THRESHOLD)
		return GameColors::water;

	if (vertexHeight < CLAY_THRESHOLD)
		return GameColors::cley;

	if (vertexHeight < SAND_THRESHOLD)
		return GameColors::sand;

	if (vertexHeight < GRASS_THRESHOLD)
		return GameColors::grass;

	if (vertexHeight < ROCK_THRESHOLD)
		return GameColors::rock;

	if (vertexHeight < HARDROCK_THRESHOLD)
		return GameColors::hardrock;

	if (vertexHeight < SNOW_THRESHOLD)
		return GameColors::snow;

	return GameColors::ice;
}
