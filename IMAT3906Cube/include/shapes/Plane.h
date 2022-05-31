#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shape.h"

class Plane : public Shape {
private:
	Texture m_texture;
	float m_planeSize = 5.0f;
	float m_planeLevel = -2.0f;

public:
	Plane(Material mat, float size, float level);
	void init();
	void draw(Shader& shader) override;
	std::vector<float> getRawVertices() override;
	std::vector<unsigned int> getRawIndices() override;
};