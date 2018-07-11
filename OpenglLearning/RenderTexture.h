#pragma once
#include "Shader.h"

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();
	void Draw(Shader shader);
	void Begin();
	void End();
private:
	unsigned int m_fbo,m_rbo;
	unsigned int m_texture;
	unsigned int m_quad_vao;
	unsigned int m_quad_vbo;
	float m_quad_vertex[24] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	void SetUp();
};

