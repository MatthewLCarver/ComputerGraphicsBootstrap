#include "Mesh.h"
#include <gl_core_4_4.h>

/**
 * \brief Deconstructor for the Mesh class
 */
Mesh::~Mesh()
{
	// If the array does not equal zero,
	// then delete the data
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);

}

/**
 * \brief Initialises the mesh with a quad
 */
void Mesh::InitialiseQuad()
{
	// Check if the mesh is not initialised already
	assert(m_vao == 0);

	// Generate buffer
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array, this will be out mesh buffer
	glBindVertexArray(m_vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Define the 6 verticies for our two triangles to make a quad,
	// in a counter-clockwise direction
	Vertex vertices[6];
	vertices[0].position = { -0.5,  0,  0.5,  1.0f };
	vertices[1].position = {  0.5f, 0,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0, -0.5f, 1.0f };

	vertices[3].position = { -0.5f, 0, -0.5f, 1.0f };
	vertices[4].position = {  0.5f, 0,  0.5f, 1.0f };
	vertices[5].position = {  0.5f, 0, -0.5f, 1.0f };


	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0, 1}; // Bottom Left
  	vertices[1].texCoord = { 1, 1}; // Bottom Right
  	vertices[2].texCoord = { 0, 0}; // Top Left
  	vertices[3].texCoord = { 0, 0}; // Top Left 
  	vertices[4].texCoord = { 1, 1}; // Bottom Right
  	vertices[5].texCoord = { 1, 0}; // Top Right
	

	// Fill the vertex buffer
	glBufferData(
		GL_ARRAY_BUFFER, 6 * sizeof(Vertex), 
		vertices, GL_STATIC_DRAW);

	// Now we will enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable the second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// Enable the third element as the texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// Next we unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// This is a quad make up of two triangles
	m_triCount = 2;
}

/**
 * \brief Initialises the mesh with a fullscreen quad for post processing
 */
void Mesh::InitialiseFullscreenQuad()
{
	// Check if the mesh is not initialised already
	assert(m_vao == 0);

	// Generate buffer
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array, this will be out mesh buffer
	glBindVertexArray(m_vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	float vertices[] = {
		-1,  1, // Top Left
		-1, -1, // Bottom Left
		 1,  1, // Top Right
		-1, -1, // Bottom Left
		 1, -1, // Bottom Right
		 1,  1  // Top Right
	};

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vertex), 
		vertices, GL_STATIC_DRAW);

	// Now we will enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

	// Next we unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_triCount = 2;
		
}

/**
 * \brief Initialises the mesh for a bespoke model
 * \param _vertexCount Set the number of vertices
 * \param _vertices Set the vertices
 * \param _indexCount Set the number of indices
 * \param _indices Set the indices
 */
void Mesh::Initialise(unsigned _vertexCount, const Vertex* _vertices, unsigned _indexCount, unsigned* _indices)
{
	// Check if the mesh is not initialised already
	assert(m_vao == 0);

	// Generate buffer
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array, this will be out mesh buffer
	glBindVertexArray(m_vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, _vertexCount * sizeof(Vertex), _vertices, GL_STATIC_DRAW);

	// Enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable the second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);
	

	// Bind the indices if there are any defined
	if(_indexCount != 0)
	{
		glGenBuffers(1, &m_ibo);

		// Bind the vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// Fill the vertex buffer
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			_indexCount * sizeof(unsigned int),
			_indices,
			GL_STATIC_DRAW);

		m_triCount = _indexCount / 3;
	}
	else
	{
		m_triCount = _vertexCount / 3;
	}

	// Unbind our buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * \brief Draws the mesh
 */
void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	// Check if we are using indicies, or just vertex points
	m_ibo != 0 ? 
		glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0) :
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
}


