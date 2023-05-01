#pragma once

constexpr int MAP_HEIGHT = 6;
constexpr int MAP_WIDTH = 6;

constexpr int MIN_VERTEX_HEIGHT = 0;
constexpr int MAX_VERTEX_HEIGHT = 100;

constexpr int NUM_STRIPS = MAP_HEIGHT - 1;
constexpr int NUM_VERTS_PER_STRIP = MAP_WIDTH * 2;

class HeighMap
{
	using vertex_color = vertex_struct_color<float>;

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
		// ---- Map generation
		const Color<float> vertexColor = { 1, 0, 1, 1 };

		std::vector<float> vertices;
		float yScale = 64.0f / 256.0f, yShift = 16.0f;  // apply a scale+shift to the height data
		for (unsigned int i = 0; i < MAP_HEIGHT; i++)
		{
			for (unsigned int j = 0; j < MAP_WIDTH; j++)
			{
				double currentHeight = 0.001f * i + (j + MAP_WIDTH * i);// *nChannels;

				// vertex
				vertices.push_back(-MAP_HEIGHT / 2.0f + i);					// v.x
				vertices.push_back((int)currentHeight);// *yScale - yShift);  // v.y
				vertices.push_back(-MAP_WIDTH / 2.0f + j );					// v.z
			}
		}

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < MAP_HEIGHT - 1; i++)       // for each row a.k.a. each strip
		{
			for (unsigned int j = 0; j < MAP_WIDTH; j++)      // for each column
			{
				for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
				{
					indices.push_back(j + MAP_WIDTH * (i + k));
				}
			}
		}

		for (int i = 0; i < MAP_HEIGHT; ++i)
		{
			for (int j = 0; j < MAP_WIDTH; ++j)
			{
				const float testReliefs = 0.01f;// *(i + j / (i + 1)); // Just making weird calculation to draw mountains
				m_points[i + j].point = Point3Df(i, testReliefs, j);
				m_points[i + j].color = vertexColor;
			}
		}

		m_numberVertices = vertices.size();
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),       // size of vertices buffer
			&vertices[0],                          // pointer to first element
			GL_STATIC_DRAW);

		//m_numberVertices = m_points.size();
		//glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_color), 0); //Pour 3D
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_color), (char*)(0) + sizeof(vertex_color::point));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			indices.size() * sizeof(unsigned int), // size of indices buffer
			&indices[0],                           // pointer to first element
			GL_STATIC_DRAW);

		glEnable(GL_DEPTH_TEST);
	}

	void render(const Mat4<float> viewProjection)
	{
		// draw mesh
		glBindVertexArray(m_vao);

		//auto mvpLocation = glGetUniformLocation(m_program, "model");
		//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, viewProjection.data());

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
	}

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_program;

	GLsizei m_numberVertices;

	std::array<vertex_color, static_cast<size_t>(MAP_HEIGHT* MAP_WIDTH)> m_points;
};