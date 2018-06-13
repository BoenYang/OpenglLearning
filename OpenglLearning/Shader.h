#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:

	GLuint shaderProgram;

	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);

	void Use();

	void SetFloat(const std::string &name, float v);

	void SetVec2(const std::string &name, float v1, float v2);

	void SetVec3(const std::string &name, float v1, float v2,float v3);

	void SetVec4(const std::string &name, float v1, float v2, float v3, float v4);

	void SetInt(const std::string &name, int v);

	void SetMatrix(const std::string &name, float *mat);

	~Shader();
};

