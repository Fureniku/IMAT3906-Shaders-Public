#include "lights/Light.h"

void Light::setAttnValues(float kc, float kl, float ke) {
	m_Kc = kc;
	m_Kl = kl;
	m_Ke = ke;
}

void Light::updateValue(Shader& shader, int id, std::string valName, float value) {
	shader.setFloat(conc(m_name, id, valName), value);
}

void Light::updateValue(Shader& shader, int id, std::string valName, glm::vec3 value) {
	shader.setVec3(conc(m_name, id, valName), value);
}

void Light::updateValue(Shader& shader, std::string valName, float value) {
	shader.setFloat(conc(m_name, valName), value);
}

void Light::updateValue(Shader& shader, std::string valName, glm::vec3 value) {
	shader.setVec3(conc(m_name, valName), value);
}

std::string Light::conc(std::string name, int id, std::string value) {
	return name + "[" + std::to_string(id) + "]." + value;
}

std::string Light::conc(std::string name, std::string value) {
	return name + "." + value;
}