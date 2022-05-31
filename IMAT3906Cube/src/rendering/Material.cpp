#include "rendering/Material.h"

void Material::init(Shader& shader) {
	shader.use();
	shader.setInt("mat.diffuseTexture", m_texture.getId());
	shader.setInt("mat.normalTexture", m_texture.getId() + 1);
	shader.setInt("mat.specularTexture", m_texture.getId() + 2);
	shader.setInt("mat.displacementTexture", m_texture.getId() + 3);
	
}

void Material::draw(Shader& shader) {
	shader.use();
	shader.setInt("mat.diffuseTexture", m_texture.getId());
	shader.setInt("mat.normalTexture", m_texture.getId() + 1);
	shader.setInt("mat.specularTexture", m_texture.getId() + 2);
	shader.setInt("mat.displacementTexture", m_texture.getId() + 3);
	shader.setFloat("mat.shine", m_shine);
	shader.setFloat("mat.PXscale", m_PXscale);
	m_texture.draw(shader);
}