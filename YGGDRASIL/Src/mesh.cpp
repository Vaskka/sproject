#include "mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

CMesh::CMesh(std::vector<SVertex> vVertices, std::vector<GLuint> vIndices, std::vector<STexture> vTextures) :m_Vertices(vVertices), m_Indices(vIndices), m_Textures(vTextures), m_VAO(0), m_VBO(0), m_EBO(0)
{
	__setupMesh();
}

CMesh::~CMesh()
{

}

//*********************************************************************************
//FUNCTION:
void CMesh::draw(GLuint vShaderProgram)
{
	GLuint DiffuseNr = 1;
	GLuint SpecularNr = 1;
	for (GLuint i = 0; i < m_Textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream SStream;
		std::string Number;
		std::string Name = m_Textures[i].Type;
		SStream << Name;
		if (Name == "uTextureDiffuse")
		{
			SStream << DiffuseNr++;
		}
		else if (Name == "uTextureSpecular")
		{
			SStream << SpecularNr++;
		}
		glUniform1i(glGetUniformLocation(vShaderProgram, SStream.str().c_str()), i);

		glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
	}

	glUniform1f(glGetUniformLocation(vShaderProgram, "uMaterialShinness"), 4.0f);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	for (GLuint i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//*********************************************************************************
//FUNCTION:
void CMesh::__setupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(SVertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (GLvoid*)offsetof(SVertex, TexCoords));

	glBindVertexArray(0);
}