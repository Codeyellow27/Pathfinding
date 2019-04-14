#include "Plane.h"

Plane::Plane()
{
	vertices = new float[12];
	vertices[0] = 0.5f; vertices[1] = 0.5f; vertices[2] = 0.0f;
	vertices[3] = 0.5f; vertices[4] = -0.5f; vertices[5] = 0.0f;
	vertices[6] = -0.5f; vertices[7] = -0.5f; vertices[8] = 0.0f;
	vertices[9] = -0.5f; vertices[10] = 0.5f; vertices[11] = 0.0f;

	indices = new int[6];
	indices[0] = 0; indices[1] = 1; indices[2] = 3;
	indices[3] = 1; indices[4] = 2; indices[5] = 3;
		
}

Plane::~Plane()
{
	delete vertices;
	delete indices;
}

size_t Plane::VerticeSize() const
{
	return sizeof(float) * 12;
}

size_t Plane::IndiceSize() const
{
	return sizeof(indices) * 6;
}

void Plane::SetShader(SimpleShader shader)
{
	shapeShader = new SimpleShader(shader);
	shapeShader->EnableBuffer(vertices, VerticeSize(), indices, IndiceSize());
}

void Plane::Draw()
{
	glBindVertexArray(shapeShader->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}