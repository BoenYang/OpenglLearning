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

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

GLuint load_texture(char const* path);

Camera camera(glm::vec3(0.0f,0.0f,6.0f));

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	Shader shader("standershader.vs", "standershader.fs");
	Model ourModel("nanosuit/nanosuit.obj");

	while(!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();

		shader.SetMatrix("projection", &projection[0][0]);
		shader.SetMatrix("view", &view[0][0]);
		shader.SetVec3("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);
		
		shader.SetVec3("ambientColor", ambientColor.r, ambientColor.g, ambientColor.b);
		shader.SetFloat("ambientStrength", ambientStrength);

		shader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.SetVec3("dirLight.color", 0.5f, 0.5f, 0.5f);

		shader.SetFloat("material.shiniess", 64);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -2.25f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	
		shader.SetMatrix("model", &model[0][0]);
		ourModel.Draw(shader);

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

	//camera.ProcessMouse(-xoffset, yoffset, deltaTime);
}

GLuint load_texture(char const* path)
{
	GLuint texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		
		GLenum format;

		if(nrChannels == 1)
		{
			format = GL_RED;
		}else if(nrChannels == 3)
		{
			format = GL_RGB;
		}else if(nrChannels == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	stbi_image_free(data);

	return texture;
}