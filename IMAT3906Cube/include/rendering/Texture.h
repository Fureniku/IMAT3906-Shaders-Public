#pragma once
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include "Shader.h"

class Texture {
private:
	unsigned int diffuse;
	unsigned int specular;
	unsigned int normal;
	unsigned int displacement;
	std::string m_path;

	int m_id;
	int m_glId;

public:
	Texture() {}
	Texture(std::string path, int id);
	unsigned int loadTexture(char const* path);
	inline int getId() { return m_id; }
	void draw(Shader& shader);
};