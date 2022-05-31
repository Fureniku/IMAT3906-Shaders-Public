#include "rendering/Texture.h"

Texture::Texture(std::string path, int id) {
	m_path = path;
	m_id = id;
	m_glId = GL_TEXTURE0 + id;
	diffuse = loadTexture((path + "\\diffuse.jpg").c_str());
	normal = loadTexture((path + "\\normal.jpg").c_str());
	specular = loadTexture((path + "\\specular.jpg").c_str());
	displacement = loadTexture((path + "\\displacement.jpg").c_str());
	
}

unsigned int Texture::loadTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format = GL_RED;
		if (nrComponents == 3) {
			format = GL_RGB;
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout << "loaded texture from path " << path << std::endl;
	}
	else {
		std::cout << "Failed to load texture from path " << path << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}

void Texture::draw(Shader& shader) {
	glActiveTexture(m_glId);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glActiveTexture(m_glId + 1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(m_glId + 2);
	glBindTexture(GL_TEXTURE_2D, specular);
	glActiveTexture(m_glId + 3);
	glBindTexture(GL_TEXTURE_2D, displacement);
}