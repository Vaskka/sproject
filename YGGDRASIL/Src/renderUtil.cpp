#include "renderUtil.h"
#include <gl/glew.h>

//**********************************************************************************************
//FUNCTION:
void util::renderScreenQuad()
{
	GLuint creenQuadVAO = 0;
	GLuint creenQuadVBO = 0;
	if (creenQuadVAO == 0)
	{
		GLfloat vertices[] =
		{
			-1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			1.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
			-1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  0.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &creenQuadVAO);
		glGenBuffers(1, &creenQuadVBO);

		glBindBuffer(GL_ARRAY_BUFFER, creenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(creenQuadVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(creenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
