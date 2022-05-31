#include "rendering/Renderer.h"
#include <GLFW/glfw3.h>

void Renderer::createObjects() {
	int texTypeSize = 4;
	int texId = -texTypeSize;
	Texture cubeTexture("..\\IMAT3906Cube\\assets\\textures\\metalPlate", 0);
	Texture planeTexture("..\\IMAT3906Cube\\assets\\textures\\metalRust", 5);

	Material cubeMat(cubeTexture, 1.f);
	Material planeMat(planeTexture, 1.f, 20.f);

	cubes.push_back(Cube(cubeMat, 0.5f, glm::vec3( 2.0,  0.0,  2.0), glm::vec3(1.0)));
	cubes.push_back(Cube(cubeMat, 0.5f, glm::vec3(-2.0,  0.0, -2.0), glm::vec3(1.0)));
	cubes.push_back(Cube(cubeMat, 0.5f, glm::vec3( 0.0,  0.0, -5.0), glm::vec3(2.0), true));
	cubes.push_back(Cube(cubeMat, 0.5f, glm::vec3(-3.0, -1.0,  2.0), glm::vec3(0.75), true, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	planes.push_back(Plane(planeMat, 5.0f, -2.0f));

	createQuad();

	skybox.createSkyBox();
}

void Renderer::renderScene(Shader& cubeShader, Shader& floorShader, Camera camera) {
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

	cubeShader.use();
	cubeShader.setMat4("projection", projection);
	cubeShader.setMat4("view", view);
	cubeShader.setMat4("model", model);
	cubeShader.setVec3("viewPos", camera.Position);

	for (int i = 0; i < cubes.size(); i++) {
		cubes[i].draw(cubeShader);
	}

	floorShader.use();
	floorShader.setMat4("projection", projection);
	floorShader.setMat4("view", view);
	floorShader.setMat4("model", model);
	floorShader.setVec3("viewPos", camera.Position);

	for (int i = 0; i < planes.size(); i++) {
		planes[i].draw(floorShader);
	}	
}

void Renderer::renderSkybox(Shader& skyboxShader, Camera camera) {
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

	skybox.renderSkyBox(skyboxShader, projection, camera);
}

void Renderer::renderLights(Shader& lightShader, Camera camera) {
	// MVP 
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

	lightShader.use();
	lightShader.setMat4("projection", projection);
	lightShader.setMat4("view", view);

	if (m_props.getPL().value) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i].draw(lightShader);
		}
	}
}

void Renderer::createLights(Camera cam) {
	pointLights.push_back(PointLight(glm::vec3(5.0, 0.5, 5.0), glm::vec3(5.0, 0.0, 0.0), glm::vec3(5.0, 0.0, 0.0), glm::vec3(5.0, 0.0, 0.0)));
	pointLights.push_back(PointLight(glm::vec3(-5.0, 0.5, -5.0), glm::vec3(5.0, 5.0, 0.0), glm::vec3(5.0, 5.0, 0.0), glm::vec3(5.0, 5.0, 0.0)));
	spotLights.push_back(SpotLight(cam.Position, cam.Front, glm::vec3(1.0f, 1.0f, 1.0f), 12.5, 17.5)); //Camera torch; always register first!!
	spotLights.push_back(SpotLight(glm::vec3(4.0, 0.25, -4.0), glm::vec3(-0.75f, -1.0f, 0.75f), glm::vec3(0.0f, 0.0f, 1.0f), 12.5, 17.5));
	spotLights.push_back(SpotLight(glm::vec3(-4.0, 0.25, 4.0), glm::vec3(0.75f, -1.0f, -0.75f), glm::vec3(0.0f, 1.0f, 1.0f), 12.5, 17.5));
}

void Renderer::setUniforms(std::vector<Shader>& shaders) {
	glm::vec3 lightDir = glm::vec3(0, -1, 0);
	glm::vec3 lightCol = glm::vec3(0.5f);

	for (int i = 0; i < shaders.size(); i++) {
		Shader& s = shaders.at(i);

		s.use();

		s.setVec3("dLightCol", lightCol);
		s.setVec3("dLightDir", lightDir);
		s.setFloat("dLightShine", 128);
		s.setFloat("dLightSpecStr", 0.4);

		for (int i = 0; i < pointLights.size(); i++) {
			pointLights.at(i).setUniforms(i, s);
		}

		for (int i = 0; i < spotLights.size(); i++) {
			spotLights.at(i).setUniforms(i, s);
		}
	}
}

void Renderer::updatePerFrameUniforms(std::vector<Shader>& shaders, Camera cam) {
	for (int i = 0; i < shaders.size(); i++) {
		Shader& s = shaders.at(i);
		s.use();
		spotLights.at(0).updateValue(s, 0, "pos", cam.Position);
		spotLights.at(0).updateValue(s, 0, "dir", cam.Front);

		s.setBool(m_props.getDL().name, m_props.getDL().value);
		s.setBool(m_props.getPL().name, m_props.getPL().value);
		s.setBool(m_props.getSL().name, m_props.getSL().value);
		s.setBool(m_props.getTorch().name, m_props.getTorch().value);
		s.setBool(m_props.getNormalMap().name, m_props.getNormalMap().value);
		s.setInt(m_props.getPx().name, m_props.getPx().value);
	}
}

void Renderer::setShadowUniforms(std::vector<Shader>& shaders, glm::mat4 lsm) {
	for (int i = 0; i < shaders.size(); i++) {
		Shader s = shaders.at(i);

		s.use();
		s.setInt("pcfLevel", 2);
		s.setInt("shadowMap", 4);
		s.setMat4("lightSpaceMatrix", lsm);
	}
}

void Renderer::createQuad() {
	float quadVertices[] =
	{
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::renderQuad(Shader& shader, unsigned int textureObj) {
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Renderer::renderQuad(Shader& shader, unsigned int texture1, unsigned int texture2) {
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}