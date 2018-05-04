#pragma once

#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

class CCamera;
class CModel;
class CYGGTechnique;

class CYGGRendering
{
public:
	CYGGRendering();
	~CYGGRendering();

	void init();
	void draw();

private:
	void __initGLFW(const std::string& vWindowTitle);
	void __initShaders();
	void __initModels();

	void __geometryPass();
	void __doMovement();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
	static void __scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);

private:
	GLFWwindow*		m_pWindow;
	CModel*			m_pModel;
	CYGGTechnique*	m_pTechnique;

	GLfloat m_DeltaTime = 0.0f;
	GLfloat m_LastFrame = 0.0f;
};