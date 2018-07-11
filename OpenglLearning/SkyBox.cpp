#include <iostream>

#include "Camera.h"
#include "SkyBox.h"
#include "stb_image.h"


SkyBox::SkyBox(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back)
{
	m_textures_path.push_back(right);
	m_textures_path.push_back(left);
	m_textures_path.push_back(top);
	m_textures_path.push_back(bottom);
	m_textures_path.push_back(front);
	m_textures_path.push_back(back);

	this->SetUp();
}

SkyBox::SkyBox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
{
	m_textures_path.push_back(right);
	m_textures_path.push_back(left);
	m_textures_path.push_back(top);
	m_textures_path.push_back(bottom);
	m_textures_path.push_back(front);
	m_textures_path.push_back(back);

	this->SetUp();
}

SkyBox::~SkyBox()
{

}

void SkyBox::Draw(Shader shader)
{
	shader.Use();
	Camera camera = Camera::main;

	shader.SetMatrix("projection", camera.GetProjectionMatrix());
	shader.SetMatrix("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(m_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void SkyBox::SetUp()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < m_textures_path.size(); i++)
	{
		unsigned char *data = stbi_load(m_textures_path[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "load image " << m_textures_path[i] << " failed" << std::endl;
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), &m_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}
