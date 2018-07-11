#pragma once
#include "Shader.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
private:
	void Render(Shader shader);
	void SetUpRender();
};

