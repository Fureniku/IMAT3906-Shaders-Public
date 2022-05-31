#pragma once
#include "Light.h"

class SpotLight : public Light {
private:
	glm::vec3 m_lightDir;
	glm::vec3 m_lightCol;

	float m_innerRad = glm::cos(glm::radians(12.5f));
	float m_outerRad = glm::cos(glm::radians(17.5f));

public:
	SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float innerRad, float outerRad) :
		Light("sLight", pos, 1.0f, 0.027f, 0.0028f, 128, 0.4),
		m_lightDir(dir),
		m_lightCol(col),
		m_innerRad(glm::cos(glm::radians(innerRad))),
		m_outerRad(glm::cos(glm::radians(outerRad)))
	{}

	void setUniforms(int id, Shader& shader) override;

	inline glm::vec3 getLookDir() {	return m_lightDir; }
};