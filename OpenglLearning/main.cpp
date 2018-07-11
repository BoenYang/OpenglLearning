#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Model.h"
#include "Cube.h"
#include "global.h"
#include "RenderTexture.h"
#include "SkyBox.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

GLuint load_cubemap(vector<string> faces);

Camera camera(glm::vec3(0.0f,0.0f,6.0f));

Camera Camera::main = camera;



float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool mouseFirstMove = true;
float lastMouseX;
float lastMouseY;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 ambientColor(1.0f,1.0f,1.0f);
float ambientStrength = 1.0f;

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpengl", NULL, NULL);
	if(window == NULL)
	{
		cout << "Failed to create glfw windows" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "failed to init glad" << endl;
		return -1;
	}


	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	glEnable(GL_DEPTH_TEST);



	Shader skyBoxShader("skyboxshader.vs", "skyboxshader.fs");
	Shader shader("standershader.vs", "standershader.fs");
	Shader screenShader("screenshader.vs","screenshader.fs");

	SkyBox skybox("skybox/right.jpg", "skybox/left.jpg", "skybox/top.jpg", "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg");

	Model ourModel("nanosuit/nanosuit.obj");
	ourModel.transform.position = glm::vec3(0.0f, -2.25f, 0.0f);
	ourModel.transform.localScale = glm::vec3(0.3f, 0.3f, 0.3f);

	Cube cube;
	cube.SetTexture("wall.jpg", "texture_diffuse");
	cube.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	ourModel.transform.localScale = glm::vec3(0.3f, 0.3f, 0.3f);

	RenderTexture rt;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	while(!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		rt.Begin();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();


		shader.Use();
		shader.SetMatrix("projection", projection);
		shader.SetMatrix("view", view);
		shader.SetVec3("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);
		shader.SetVec3("ambientColor", ambientColor.r, ambientColor.g, ambientColor.b);
		shader.SetFloat("ambientStrength", ambientStrength);
		shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.SetVec3("dirLight.color", 0.5f, 0.5f, 0.5f);
		shader.SetFloat("material.shiniess", 64);

		ourModel.transform.eulersRoataion = glm::vec3(0, 1 * glfwGetTime(), 0);
		ourModel.Draw(shader);

		cube.Draw(shader);

		skybox.Draw(skyBoxShader);

		rt.End();
		glDisable(GL_DEPTH_TEST);

		screenShader.Use();
		rt.Draw(screenShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyBoard(Camera_Movement::FORWARD, deltaTime);
	}
	else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyBoard(Camera_Movement::BACKWARD, deltaTime);
	}
	else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyBoard(Camera_Movement::LEFT, deltaTime);
	}
	else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyBoard(Camera_Movement::RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if(mouseFirstMove)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		mouseFirstMove = false;
	}

	float xoffset = xpos - lastMouseX;
	float yoffset = ypos - lastMouseY;

	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.ProcessMouse(-xoffset, yoffset, deltaTime);
}
