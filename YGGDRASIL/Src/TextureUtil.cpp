#include "TextureUtil.h"
#include <iostream>
#include "common/HiveCommonMicro.h"
#include "common/CommonInterface.h"
#include "stb_image.h"

//**********************************************************************************************
//FUNCTION:
GLuint util::loadCubemap(const std::vector<std::string>& vFaces, bool vGenerateMipMap)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);

	int Width, Height, NrComponents;
	float *pImageData = nullptr;

	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	for (GLuint i = 0; i < vFaces.size(); ++i)
	{
		pImageData = stbi_loadf(vFaces[i].c_str(), &Width, &Height, &NrComponents, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, pImageData);
		stbi_image_free(pImageData);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	if (vGenerateMipMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return TextureID;
}

//**********************************************************************************************
//FUNCTION:
GLuint util::setupCubemap(int vWidth, int vHeight, bool vGenerateMipMap)
{
	GLuint Cubemap;
	glGenTextures(1, &Cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, vWidth, vHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	if (vGenerateMipMap)
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return Cubemap;
}

//**********************************************************************************************
//FUNCTION:
GLuint util::loadTexture(const char *vPath, GLint vWrapMode, GLint vFilterMode, bool vFlipVertically)
{
	stbi_set_flip_vertically_on_load(vFlipVertically);

	int Width, Height, Channels;
	unsigned char *pImageData = stbi_load(vPath, &Width, &Height, &Channels, 0);
	_HIVE_EARLY_RETURN(!pImageData, "Failed to load texture due to failure of stbi_load().", -1);

	GLenum Format;
	if (Channels == 1) Format = GL_RED;
	else if (Channels == 3) Format = GL_RGB;
	else if (Channels == 4) Format = GL_RGBA;

	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	switch (Format)
	{
	case GL_RED:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, pImageData);
		break;
	case GL_RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
		break;
	case GL_RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
		break;
	default:
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, vFilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, vFilterMode);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(pImageData);

	return TextureID;
}

//**********************************************************************************************
//FUNCTION:
GLuint util::setupTexture(unsigned int vWidth, unsigned int vHeight, GLint vInternalFormat, GLenum vFormat, bool vGenerateMipMap)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, vInternalFormat, vWidth, vHeight, 0, vFormat, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (vGenerateMipMap == false)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
