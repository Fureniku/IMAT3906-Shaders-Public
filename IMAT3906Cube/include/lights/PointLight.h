#pragma once
#include "Light.h"
#include "shapes/Cube.h"

class PointLight : public Light {
private:
	glm::vec3 m_aLightCol;
	glm::vec3 m_dLightCol;
	glm::vec3 m_sLightCol;

	Cube cube;

public:
	PointLight(glm::vec3 pos, glm::vec3 aCol, glm::vec3 dCol, glm::vec3 sCol) :
		Light("pLight", pos, 1.0f, 0.22f, 0.22f, 128, 0.4),
		m_aLightCol(aCol),
		m_dLightCol(dCol),
		m_sLightCol(sCol) {
		cube = Cube(aCol, 0.25f, pos, glm::vec3(1.0f));
	}

	void setUniforms(int id, Shader& shader) override;
	void draw(Shader& shader);
};