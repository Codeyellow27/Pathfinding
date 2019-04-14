#include "Line.h"

Line::Line()
{
	vertices = new float[6];
	vertices[0] = 0.5f; vertices[1] = 0.0f; vertices[2] = 0.0f;
	vertices[3] = -0.5f; vertices[4] = 0.0f; vertices[5] = 0.0f;

	indices = new int[2];
	indices[0] = 0; indices[1] = 1;

}

void Line::SetLine(const glm::vec3 a, const glm::vec3 b)
{
	vertices[0] = a.x; vertices[1] = a.y; vertices[2] = a.z;
	vertices[3] = b.x; vertices[4] = b.y; vertices[5] = b.z;
}

Line::~Line()
{
	delete vertices;
	delete indices;
}

size_t Line::VerticeSize() const
{
	return sizeof(float) * 6;
}

size_t Line::IndiceSize() const
{
	return sizeof(indices) * 2;
}

void Line::SetShader(SimpleShader shader)
{
	shapeShader = new SimpleShader(shader);
	shapeShader->EnableBuffer(vertices, VerticeSize(), indices, IndiceSize());
}



void Line::Draw()
{
	glBindVertexArray(shapeShader->VAO);
	glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}