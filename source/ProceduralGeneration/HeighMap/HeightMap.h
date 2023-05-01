#pragma once

constexpr int MAP_HEIGHT = 100;
constexpr int MAP_WIDTH = 100;

constexpr int MIN_VERTEX_HEIGHT = 0;
constexpr int MAX_VERTEX_HEIGHT = 20;

class HeighMap
{
	using vertex_colored = vertex_struct_color<float>;

public:
	HeighMap()
	{
		load();
	}

	~HeighMap()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void load()
	{
		constexpr float TERRAIN_WAVE_AMPLITUDE = MAX_VERTEX_HEIGHT / 2.f;
		constexpr float TERRAIN_WAVE_FREQUENCY = 1 / 8.f;
		constexpr int MAP_POS_Y_OFFSET = -40;

		for (unsigned int i = 0; i < MAP_HEIGHT; i++)
		{
			for (unsigned int j = 0; j < MAP_WIDTH; j++)
			{
				const float vertexHeight = std::sin(i * TERRAIN_WAVE_FREQUENCY) * TERRAIN_WAVE_AMPLITUDE + TERRAIN_WAVE_AMPLITUDE;

				// new vertex
				vertex_colored newVtColored;
				newVtColored.point = Point3Df(
					-MAP_HEIGHT / 2.0f + i,
					vertexHeight + MAP_POS_Y_OFFSET,
					-MAP_WIDTH / 2.0f + j);

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

	void render(const Mat4<float> viewProjection)
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

	static Color<float> getColorFromVertexHeight(float vertexHeight)
	{
		constexpr float ABYSSWATER_THRESHOLD	= 0.03f;
		constexpr float DEEPWATER_THRESHOLD		= 0.08f;
		constexpr float WATER_THRESHOLD			= 0.18f;
		constexpr float CLAY_THRESHOLD			= 0.22f;
		constexpr float SAND_THRESHOLD			= 0.3f;
		constexpr float GRASS_THRESHOLD			= 0.6f;
		constexpr float ROCK_THRESHOLD			= 0.75f;
		constexpr float HARDROCK_THRESHOLD		= 0.85f;
		constexpr float SNOW_THRESHOLD			= 0.95f;

		vertexHeight = MathUtils::remapValue(vertexHeight, MIN_VERTEX_HEIGHT, MAX_VERTEX_HEIGHT, 0, 1);

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

private:
	GLuint m_vao { 0 };
	GLuint m_vbo{ 0 };
	GLuint m_ebo{ 0 };
	GLuint m_program{ 0 };

	std::vector<vertex_colored> m_points;
};