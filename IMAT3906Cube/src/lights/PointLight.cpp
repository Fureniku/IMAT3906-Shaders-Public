#include "lights/PointLight.h"

void PointLight::setUniforms(int id, Shader& shader) {
	shader.setVec3(conc(m_name, id, "pos"), m_lightPos);
	shader.setVec3(conc(m_name, id, "aCol"), m_aLightCol);
	shader.setVec3(conc(m_name, id, "dCol"), m_dLightCol);
	shader.setVec3(conc(m_name, id, "sCol"), m_sLightCol);
	shader.setFloat(conc(m_name, id, "Kc"), m_Kc);
	shader.setFloat(conc(m_name, id, "Kl"), m_Kl);
	shader.setFloat(conc(m_name, id, "Ke"), m_Ke);

	shader.setFloat(conc(m_name, id, "shine"), m_shine);
	shader.setFloat(conc(m_name, id, "specStr"), m_specularStrength);
}

void PointLight::draw(Shader& shader) {
	cube.draw(shader);
}
