#include "lights/SpotLight.h"

void SpotLight::setUniforms(int id, Shader& shader) {
	shader.setVec3(conc(m_name, id, "pos"), m_lightPos);
	shader.setVec3(conc(m_name, id, "dir"), m_lightDir);
	shader.setVec3(conc(m_name, id, "col"), m_lightCol);
	
	shader.setFloat(conc(m_name, id, "Kc"), m_Kc);
	shader.setFloat(conc(m_name, id, "Kl"), m_Kl);
	shader.setFloat(conc(m_name, id, "Ke"), m_Ke);

	shader.setFloat(conc(m_name, id, "innerRad"), m_innerRad);
	shader.setFloat(conc(m_name, id, "outerRad"), m_outerRad);

	shader.setFloat(conc(m_name, id, "shine"), m_shine);
	shader.setFloat(conc(m_name, id, "specStr"), m_specularStrength);
}