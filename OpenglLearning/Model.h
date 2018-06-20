#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"


using namespace std;

class Model
{
public:
	vector<Mesh> meshes;
	string directory;
	Model(char* path);
	~Model();
	void Draw(Shader shader);
private:
	void loadModel(string path);
	void processNode(aiNode *node,const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,string typeName);
};

