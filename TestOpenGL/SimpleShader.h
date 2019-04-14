#pragma once

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"


class SimpleShader : public Shader
{
	public:

		SimpleShader(const std::string vsPath, const std::string fsPath);

		std::string vsShaderPath;
		std::string fsShaderPath;

		GLuint VBO;
		GLuint VAO;
		GLuint EBO;

		void EnableBuffer(float* vertices, size_t verticeSize, int* indices, size_t indiceSize);
		void UseShader();
		void BindUniform(glm::mat4 transformMatrix);

		void DeleteBuffer();



	
};