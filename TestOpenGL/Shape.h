#pragma once

#include "SimpleShader.h"

class Shape
{
	public:
		Shape();
		~Shape();

		SimpleShader* shapeShader;
		glm::vec3 shapeColor;
		glm::mat4 transform;

		void ShaderUse();
		virtual void SetShader(SimpleShader shader) = 0;

		void SetShaderBool(const std::string &name, bool value) const;
		void SetShaderInt(const std::string& name, int value) const;
		void SetShaderFloat(const std::string& name, float value) const;
		void SetVec3(const std::string& name, glm::vec3 value) const;
		void SetShaderMatrix(const std::string& name, glm::mat4 value) const;

		void SetColor(const glm::vec3 color);
			
		virtual void Draw() = 0;

};