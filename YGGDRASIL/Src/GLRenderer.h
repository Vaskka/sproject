#pragma once

#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

class CGLRenderer
{
public:
	CGLRenderer();
	virtual ~CGLRenderer();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight);
	virtual void runV();

protected:
	virtual void _drawV();
	virtual void _handleEventsV();

protected:
	GLFWwindow *m_pWindow;
	GLfloat m_DeltaTime;
	GLfloat m_LastFrame;
};