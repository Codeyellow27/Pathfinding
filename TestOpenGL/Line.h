#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shape.h"

class Line : public Shape
{
public:
	Line();
	~Line();

	float* vertices;
	int* indices;

	size_t VerticeSize() const;
	size_t IndiceSize() const;

	void SetShader(SimpleShader shader) override;
	void Draw() override;

	void SetLine(const glm::vec3 a, const glm::vec3 b);
};