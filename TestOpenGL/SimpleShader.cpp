#include "SimpleShader.h"

SimpleShader::SimpleShader(const std::string vsPath, const std::string fsPath)
	: vsShaderPath(vsPath), fsShaderPath(fsPath),Shader(vsPath.c_str(),fsPath.c_str())
{

}

void SimpleShader::EnableBuffer(float* vertices, size_t verticeSize, int* indices, size_t indiceSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticeSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void SimpleShader::UseShader()
{
	Use();
}

void SimpleShader::BindUniform(glm::mat4 transformMatrix)
{
	unsigned int transformLoc = glGetUniformLocation(id, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));
}

void SimpleShader::DeleteBuffer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}