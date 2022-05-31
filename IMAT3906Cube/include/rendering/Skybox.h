#pragma once
#include <stb_image.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "rendering/Shader.h"
#include "Camera.h"

class Skybox {
private:
	std::vector<std::string> faces;
	unsigned int skyboxTexture;
	unsigned int skyboxVAO;

	void loadTextures();
	void createVAO();

public:
	Skybox() {}
	void createSkyBox();
	unsigned int loadCubeMap(std::vector<std::string> faces);
	void renderSkyBox(Shader& shader, glm::mat4 projection, Camera camera);
};