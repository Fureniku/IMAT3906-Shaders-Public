#pragma once
#include <vector>

#include "shapes/Cube.h"
#include "shapes/Plane.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "Shader.h"
#include "Camera.h"
#include "Properties.h"
#include "Skybox.h"

class Renderer {

private:
	std::vector<Cube> cubes;
	std::vector<Plane> planes;

	std::vector<PointLight> pointLights;
	std::vector<SpotLight> spotLights;
	//Quad m_quad;
	unsigned int quadVAO;
	unsigned int quadVBO;

	bool wireframe = false;

	// settings
	unsigned int screenWidth, screenHeight;
	Properties m_props;
	Skybox skybox;
public:
	Renderer(unsigned int w, unsigned int h, Properties props) :
		screenWidth(w),
		screenHeight(h),
		m_props(props) {}
	void renderScene(Shader& cubeShader, Shader& floorShader, Camera camera);
	void renderLights(Shader& lightShader, Camera camera);
	void renderSkybox(Shader& skyboxShader, Camera camera);
	void createObjects();
	void createLights(Camera cam);
	void drawLight(Shader& shader);
	void setShadowUniforms(std::vector<Shader>& shaders, glm::mat4 lsm);
	void setUniforms(std::vector<Shader>& shaders);
	void updatePerFrameUniforms(std::vector<Shader>& shaders, Camera cam);
	//inline Quad getQuad() { return m_quad; }
	void createQuad();
	void renderQuad(Shader& shader, unsigned int textureObj);
	void renderQuad(Shader& shader, unsigned int texture1, unsigned int texture2);
	inline void updateProperties(Properties props) { m_props = props; }
};