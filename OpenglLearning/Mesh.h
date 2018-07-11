#pragma once

#include <vector>
#include "Shader.h"
#include "global.h"

using namespace std;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices,vector<unsigned int> indices,vector<Texture> textures);
	void Draw(Shader shader);
	~Mesh();
private:
	unsigned int vao, vbo, ebo;
	void SetupMesh();
};

