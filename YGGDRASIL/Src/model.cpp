#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/version.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CModel::CModel()
{

}

CModel::~CModel()
{

}

//*********************************************************************************
//FUNCTION:
static GLint load_texture_from_file(const char* vPath, const std::string& vDirectory)
{
	std::string Filename = std::string(vPath);
	Filename = vDirectory + '/' + Filename;
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	int Width, Height, Channels;
	unsigned char* pImage = stbi_load(Filename.c_str(), &Width, &Height, &Channels, STBI_rgb);

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImage);
	return TextureID;
}

//*********************************************************************************
//FUNCTION:
bool CModel::load(const std::string& vPath)
{
	Assimp::Importer LocImporter;
	const aiScene* pScene = LocImporter.ReadFile(vPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << LocImporter.GetErrorString() << std::endl;
		return false;
	}
#ifdef _DEBUG
	std::cout << pScene->mNumMaterials << std::endl;
#endif

	m_Directory = vPath.substr(0, vPath.find_last_of('/'));
	__processNode(pScene->mRootNode, pScene);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CModel::draw(GLuint vShaderProgram)
{
	for (GLuint i = 0; i < this->m_Meshes.size(); i++)
		this->m_Meshes[i].draw(vShaderProgram);
}

//*********************************************************************************
//FUNCTION:
void CModel::__processNode(const aiNode* vNode, const aiScene* vScene)
{
	for (GLuint i = 0; i < vNode->mNumMeshes; ++i)
	{
		aiMesh* mesh = vScene->mMeshes[vNode->mMeshes[i]];
		m_Meshes.push_back(__processMesh(mesh, vScene));
	}

	for (GLuint i = 0; i < vNode->mNumChildren; ++i)
	{
		__processNode(vNode->mChildren[i], vScene);
	}
}

//*********************************************************************************
//FUNCTION:
CMesh CModel::__processMesh(const aiMesh* vMesh, const aiScene* vScene)
{
	std::vector<SVertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector<STexture> Textures;
	SMaterial Material;

	for (GLuint i = 0; i < vMesh->mNumVertices; ++i)
	{
		SVertex TempVertex;
		glm::vec3 TempVector;

		TempVector.x = vMesh->mVertices[i].x;
		TempVector.y = vMesh->mVertices[i].y;
		TempVector.z = vMesh->mVertices[i].z;
		TempVertex.Position = TempVector;
		TempVector.x = vMesh->mNormals[i].x;
		TempVector.y = vMesh->mNormals[i].y;
		TempVector.z = vMesh->mNormals[i].z;
		TempVertex.Normal = TempVector;

		if (vMesh->mTextureCoords[0])
		{
			glm::vec2 Vec;
			Vec.x = vMesh->mTextureCoords[0][i].x;
			Vec.y = vMesh->mTextureCoords[0][i].y;
			TempVertex.TexCoords = Vec;
		}
		else
			TempVertex.TexCoords = glm::vec2(0.0f, 0.0f);
		Vertices.push_back(TempVertex);
	}

	for (GLuint i = 0; i < vMesh->mNumFaces; ++i)
	{
		aiFace Face = vMesh->mFaces[i];

		for (GLuint k = 0; k < Face.mNumIndices; ++k)
			Indices.push_back(Face.mIndices[k]);
	}

	if (vMesh->mMaterialIndex >= 0)
	{
		aiMaterial* pMaterial = vScene->mMaterials[vMesh->mMaterialIndex];
#ifdef _DEBUG
		//for (int i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
		//	std::cout << pMaterial->GetTextureCount(aiTextureType(i));
#endif
		std::cout << std::endl;
		std::vector<STexture> DiffuseMaps = __loadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "uTextureAlbedo");
		Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

		aiColor3D AmbientColor;
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, AmbientColor);
		Material.Amibent = { AmbientColor.r, AmbientColor.g, AmbientColor.b };
		aiColor3D DiffuseColor;
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor);
		Material.Diffuse = { DiffuseColor.r, DiffuseColor.g, DiffuseColor.b };
	}

	return CMesh(Vertices, Indices, Textures, Material);
}

//*********************************************************************************
//FUNCTION:
std::vector<STexture> CModel::__loadMaterialTextures(const aiMaterial* vMat, aiTextureType vType, const std::string& vTypeName)
{
	std::vector<STexture> Textures;
	for (GLuint i = 0; i < vMat->GetTextureCount(vType); ++i)
	{
		aiString Str;
		vMat->GetTexture(vType, i, &Str);

		GLboolean Skip = false;
		for (GLuint j = 0; j < m_LoadedTextures.size(); j++)
		{
			if (std::strcmp(m_LoadedTextures[j].Path.C_Str(), Str.C_Str()) == 0)
			{
				Textures.push_back(m_LoadedTextures[j]);
				Skip = true;
				break;
			}
		}
		if (!Skip)
		{
			STexture TempTexture;
			TempTexture.Id = load_texture_from_file(Str.C_Str(), this->m_Directory);
			TempTexture.Type = vTypeName;
			TempTexture.Path = Str;
			Textures.push_back(TempTexture);
			this->m_LoadedTextures.push_back(TempTexture);
		}
	}
	return Textures;
}