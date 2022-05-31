#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "rendering/Renderer.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"
#include "rendering/Texture.h"
#include "rendering/FrameBuffer.h"
#include "Properties.h"

#include <string>
#include <iostream>
#include <numeric>

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// camera
Camera camera(glm::vec3(0,0,9));
Properties props;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Renderer renderer(SCR_WIDTH, SCR_HEIGHT, props);
FrameBuffer frameBuffer({ SCR_WIDTH, SCR_HEIGHT }, props);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(window, keyCallback);

	props.populate();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::string vertexShaderLoc = "..\\IMAT3906Cube\\assets\\shaders\\vertex\\";
	std::string fragmentShaderLoc = "..\\IMAT3906Cube\\assets\\shaders\\fragment\\";

	Shader cubeShader((vertexShaderLoc + "plainVert.vert").c_str(), (fragmentShaderLoc + "plainFrag.frag").c_str());
	Shader floorShader((vertexShaderLoc + "plainVert.vert").c_str(), (fragmentShaderLoc + "floorFrag.frag").c_str());
	Shader lightShader((vertexShaderLoc + "lightShader.vert").c_str(), (fragmentShaderLoc + "lightShader.frag").c_str());
	Shader colPostProcess((vertexShaderLoc + "postProcess.vert").c_str(), (fragmentShaderLoc + "colPostProcess.frag").c_str());
	Shader depthPostProcess((vertexShaderLoc + "postProcess.vert").c_str(), (fragmentShaderLoc + "depthPostProcess.frag").c_str());
	Shader blurPostProcess((vertexShaderLoc + "postProcess.vert").c_str(), (fragmentShaderLoc + "blurPostProcess.frag").c_str());
	Shader bloomPostProcess((vertexShaderLoc + "postProcess.vert").c_str(), (fragmentShaderLoc + "bloomPostProcess.frag").c_str());
	//Shader dofPostProcess((vertexShaderLoc + "postProcess.vert").c_str(), (fragmentShaderLoc + "dofPostProcess.frag").c_str());
	Shader shadowMapShader((vertexShaderLoc + "ShadowMap.vert").c_str(), (fragmentShaderLoc + "ShadowMap.frag").c_str());
	Shader skyboxShader((vertexShaderLoc + "skybox.vert").c_str(), (fragmentShaderLoc + "skybox.frag").c_str());

	colPostProcess.use();
	colPostProcess.setInt("image", 0);
	depthPostProcess.use();
	depthPostProcess.setInt("image", 0);
	blurPostProcess.use();
	blurPostProcess.setInt("image", 0);
	blurPostProcess.setInt("r", 20);
	blurPostProcess.setFloat("bias", 1.0f);
	bloomPostProcess.use();
	bloomPostProcess.setInt("image", 0);
	bloomPostProcess.setInt("bloomBlur", 1);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	cubeShader.use();

	std::vector<Shader> shaders;
	shaders.push_back(cubeShader);
	shaders.push_back(floorShader);

	renderer.createObjects();
	renderer.createLights(camera);
	renderer.setUniforms(shaders);
	frameBuffer.setFBOColAndDepth();
	frameBuffer.setFBOBlur();
	frameBuffer.setFBOShadowMap();

	while (!glfwWindowShouldClose(window)) 	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFboShadowMap());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		float oSize = 10;
		glm::vec3 lightDir = glm::vec3(0.5, -2, -2);

		glm::mat4 lightProj = glm::ortho(-oSize, oSize, -oSize, oSize, -oSize, 2 * oSize);
		glm::mat4 lightView = glm::lookAt(lightDir * glm::vec3(-1.0), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightProj * lightView;

		shadowMapShader.use();
		shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		renderer.renderScene(shadowMapShader, shadowMapShader, camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.setShadowUniforms(shaders, lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, frameBuffer.getShadowAttachment());

		renderer.renderScene(cubeShader, floorShader, camera);
		renderer.renderLights(lightShader, camera);
		

		//First pass (colour)
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFboColDepth());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		renderer.renderScene(cubeShader, floorShader, camera);
		renderer.renderLights(lightShader, camera);
		

		//Second pass (depth)
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFboDepth());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.renderScene(cubeShader, floorShader, camera);
		renderer.renderLights(lightShader, camera);

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.getFboBlur());
		glDisable(GL_DEPTH_TEST);

		blurPostProcess.use();
		blurPostProcess.setBool("horz", true);
		if (props.getBlurBloom().value == 1) {
			renderer.renderQuad(blurPostProcess, frameBuffer.getColAttArray(0));
		}
		else {
			renderer.renderQuad(blurPostProcess, frameBuffer.getColAttArray(1));
		}
		blurPostProcess.setBool("horz", false);
		renderer.renderQuad(blurPostProcess, frameBuffer.getBlurAttachment());

		//Third pass (render)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (props.getDepth().value) {
			renderer.renderQuad(depthPostProcess, frameBuffer.getDepthAttachment());
		}
		else {
			if (props.getBlurBloom().value == 0) {
				renderer.renderQuad(colPostProcess, frameBuffer.getColAttArray(props.getMultiShader().value));
			}
			else if (props.getBlurBloom().value == 1) {
				renderer.renderQuad(colPostProcess, frameBuffer.getBlurAttachment()); //blur
			}
			else if (props.getBlurBloom().value == 2) {
				renderer.renderQuad(bloomPostProcess, frameBuffer.getColAttArray(0), frameBuffer.getBlurAttachment()); //bloom
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		renderer.updatePerFrameUniforms(shaders, camera);
		colPostProcess.use();
		colPostProcess.setBool(props.getHDR().name, props.getHDR().value);
		colPostProcess.setInt(props.getColShader().name, props.getColShader().value);

		renderer.updateProperties(props);
	}

	glfwTerminate();
	return 0;
}

bool keyDown = false;

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVE_DOWN, deltaTime);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		props.setDL(!props.getDL().value);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		props.setPL(!props.getPL().value);
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		props.setSL(!props.getSL().value);
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		props.setTorch(!props.getTorch().value);
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		props.setNormalMap(!props.getNormalMap().value);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		props.setPx(props.cycleIntProp(props.getPx()));
		std::cout << props.getPx().outputs.at(props.getPx().value) << std::endl;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		props.setColShader(props.cycleIntProp(props.getColShader()));
		std::cout << props.getColShader().outputs.at(props.getColShader().value) << std::endl;
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		props.setDepth(!props.getDepth().value);
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		props.setBlurBloom(props.cycleIntProp(props.getBlurBloom()));
		std::cout << props.getBlurBloom().outputs.at(props.getBlurBloom().value) << std::endl;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		props.setMultiShader(props.cycleIntProp(props.getMultiShader()));
		std::cout << props.getMultiShader().outputs.at(props.getMultiShader().value) << std::endl;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		props.setHDR(!props.getHDR().value);
		std::cout << "HDR set to " << props.getHDR().value << std::endl;
		frameBuffer.setFBOColAndDepth();
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}