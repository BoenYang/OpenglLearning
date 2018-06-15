#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"

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
glm::vec3 ambientColor(0.5f,0.5f,0.5f);
float ambientStrength = 0.1f;
float specularStrength = 0.5f;

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

	//Shader shader("shader.vs", "shader.fs");
	Shader lightShader("materialshader.vs", "materialshader.fs");
	Shader lampShader("lampshader.vs", "lampshader.fs");


	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	GLuint lightVao;
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint diffuse = load_texture("container2.png");
	GLuint specular = load_texture("container2_specular.png");

	glm::vec3 pointLightPos[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while(!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();


		lightShader.Use();
		lightShader.SetMatrix("view", &view[0][0]);
		lightShader.SetMatrix("projection", &projection[0][0]);

		lightShader.SetVec3("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);

		lightShader.SetInt("material.diffuse", 0);
		lightShader.SetInt("material.specular", 1);
		lightShader.SetFloat("material.shiniess", 64);
		lightShader.SetFloat("material.specularStrength", specularStrength);

		lightShader.SetVec3("ambientColor", ambientColor.r, ambientColor.g, ambientColor.b);
		lightShader.SetFloat("ambientStrength", ambientStrength);

		lightShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		// point light 1
		lightShader.SetVec3("pointLights[0].position", pointLightPos[0].x, pointLightPos[0].y, pointLightPos[0].z);
		lightShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.SetFloat("pointLights[0].constant", 1.0f);
		lightShader.SetFloat("pointLights[0].linear", 0.09);
		lightShader.SetFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightShader.SetVec3("pointLights[1].position", pointLightPos[1].x, pointLightPos[1].y, pointLightPos[1].z);
		lightShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.SetFloat("pointLights[1].constant", 1.0f);
		lightShader.SetFloat("pointLights[1].linear", 0.09);
		lightShader.SetFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightShader.SetVec3("pointLights[2].position", pointLightPos[2].x, pointLightPos[2].y, pointLightPos[2].z);
		lightShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.SetFloat("pointLights[2].constant", 1.0f);
		lightShader.SetFloat("pointLights[2].linear", 0.09);
		lightShader.SetFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightShader.SetVec3("pointLights[3].position", pointLightPos[3].x, pointLightPos[3].y, pointLightPos[3].z);
		lightShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightShader.SetFloat("pointLights[3].constant", 1.0f);
		lightShader.SetFloat("pointLights[3].linear", 0.09);
		lightShader.SetFloat("pointLights[3].quadratic", 0.032);

		// spotLight
		lightShader.SetVec3("spotLight.position", camera.pos.x,camera.pos.y,camera.pos.z);
		lightShader.SetVec3("spotLight.direction", camera.front.x,camera.front.y,camera.front.z);
		lightShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightShader.SetFloat("spotLight.constant", 1.0f);
		lightShader.SetFloat("spotLight.linear", 0.09);
		lightShader.SetFloat("spotLight.quadratic", 0.032);
		lightShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightShader.SetMatrix("model", glm::value_ptr(model));

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model1;
			model1 = glm::translate(model1, pointLightPos[i]);
			model1 = glm::scale(model1, glm::vec3(0.2f));
			lampShader.Use();
			lampShader.SetMatrix("view", &view[0][0]);
			lampShader.SetMatrix("projection", &projection[0][0]);
			lampShader.SetMatrix("model", glm::value_ptr(model1));
			glBindVertexArray(lightVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


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