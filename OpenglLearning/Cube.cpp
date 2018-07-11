#include <string>

#include "Camera.h"
#include "Cube.h"
#include "TextureUtils.h"

using namespace std;

Cube::Cube()
{
	this->SetUp();
}


Cube::~Cube()
{
}

void Cube::Draw(Shader shader)
{

	Camera camera = Camera::main;
	shader.Use();
	shader.SetMatrix("model", transform.GetModelMatrix());
	shader.SetMatrix("view", camera.GetViewMatrix());
	shader.SetMatrix("projection", camera.GetProjectionMatrix());
	shader.SetVec3("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = m_textures[i].type;

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}

		shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(m_vertex));
	glBindVertexArray(0);
}

void Cube::SetTexture(std::string path, std::string type)
{
	Texture texture;
	texture.id = TextureUtils::LoadTexture(path.c_str());
	texture.type = type;
	texture.path = path;
	m_textures.push_back(texture);
}

void Cube::SetUp()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}
