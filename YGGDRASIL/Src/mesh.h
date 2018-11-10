#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include "Shader.h"

typedef struct
{
	GLuint Id;
	std::string Type;
	aiString Path;
} STexture;

typedef struct
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
} SVertex;

typedef struct
{
	glm::vec3 Amibent;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shinness;
} SMaterial;

class CMesh
{
public:
	CMesh(std::vector<SVertex> vVertices, std::vector<GLuint> vIndices, std::vector<STexture> vTextures, const SMaterial& vMaterial);
	~CMesh();

	void draw(GLuint vShaderProgram);

private:
	std::vector<SVertex> m_Vertices;
	std::vector<GLuint> m_Indices;
	std::vector<STexture> m_Textures;
	SMaterial m_Material;
	GLuint m_VAO, m_VBO, m_EBO;

	void __setupMesh();
};


