#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <assimp/scene.h>
#include "mesh.h"

class CModel
{
public:
	CModel();
	~CModel();

	bool load(const std::string& vPath);
	void draw(GLuint vShaderProgram);

private:
	void __processNode(const aiNode* vNode, const aiScene* vScene);
	CMesh __processMesh(const aiMesh* vMesh, const aiScene* vScene);
	std::vector<STexture> __loadMaterialTextures(const aiMaterial* vMat, aiTextureType vType, const std::string& vTypeName);

private:
	std::vector<CMesh> m_Meshes;
	std::string m_Directory;
	std::vector<STexture> m_LoadedTextures;
};
