#include "GLRenderer.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>

CGLRenderer::CGLRenderer()
{

}

CGLRenderer::~CGLRenderer()
{

}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen /*= false*/)
{
	if (m_IsInitialized) return true;

	_ASSERTE(vWindowWidth > 0 && vWindowHeight > 0);
	_HIVE_EARLY_RETURN(!__createGLFWWindow(vWindowTitle, vWindowWidth, vWindowHeight, vIsFullScreen), "Fail to initialize opengl renderer due to failure of __createGLFWWindow().", false);

	glEnable(GL_DEPTH_TEST);

	m_IsInitialized = true;
	return true;
}

//*********************************************************************************
//FUNCTION:
int CGLRenderer::runV()
{
	while (!m_IsRenderLoopDone)
	{
		_handleEventsV();
		_renderV();

		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);

		m_IsRenderLoopDone = _isRenderLoopDoneV();
		__updateFrameInterval();
	}

	glfwTerminate();
	return 0;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::_isRenderLoopDoneV()
{
	bool IsRenderLoopDone = glfwWindowShouldClose(m_pWindow);
	if (IsRenderLoopDone) glfwTerminate();
	return IsRenderLoopDone;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderer::__createGLFWWindow(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen /*= false*/)
{
	_HIVE_EARLY_RETURN(!glfwInit(), "Fail to initialize glfw due to failure of glfwInit().", false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_ASSERTE(!m_pWindow);
	GLFWmonitor *pMonitor = vIsFullScreen ? glfwGetPrimaryMonitor() : nullptr;
	m_pWindow = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowTitle.c_str(), pMonitor, nullptr);
	if (!m_pWindow)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to create window due to failure of glfwCreateWindow()");
		glfwTerminate();
		return false;
	}
	glfwSetWindowPos(m_pWindow, 0, 0); //TODO: ø…≈‰÷√≥ı ºŒª÷√
	glfwMakeContextCurrent(m_pWindow);
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	_HIVE_EARLY_RETURN((glewInit() != GLEW_OK), "Fail to initialize glew due to failure of glewInit().", false);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CGLRenderer::__updateFrameInterval()
{
	double CurrentFrameTime = static_cast<GLfloat>(glfwGetTime());
	static double LastFrameTime = 0.0;
	m_FrameInterval = CurrentFrameTime - LastFrameTime;
	LastFrameTime = CurrentFrameTime;
}
