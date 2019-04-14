#include "Triangle.h"

Triangle::Triangle()
{
	vertices = new float[9];
	vertices[0] = -0.5f; vertices[1] = -0.5f; vertices[2] = 0.0f;
	vertices[3] = 0.5f; vertices[4] = -0.5f; vertices[5] = 0.0f;
	vertices[6] = 0.0f; vertices[7] = 0.5f; vertices[8] = 0.0f;

	indices = new int[3];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
}

Triangle::~Triangle()
{
	delete vertices;
	delete indices;
}

size_t Triangle::VerticeSize() const
{
	return sizeof(float) * 9;
}

size_t Triangle::IndiceSize() const
{
	return sizeof(indices) * 3;
}

void Triangle::SetShader(SimpleShader shader)
{
	shapeShader = new SimpleShader(shader);
	shapeShader->EnableBuffer(vertices, VerticeSize(), indices, IndiceSize());
}

void Triangle::Draw()
{
	glBindVertexArray(shapeShader->VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}