#include "GLRenderEngine.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>
#include "RenderEngineInterface.h"
#include "DisplayDevice.h"

using namespace sengine::srenderer;

hiveOO::CProductFactory<CGLRenderEngine> Creator("srendererGL");

CGLRenderEngine::CGLRenderEngine()
{

}

CGLRenderEngine::~CGLRenderEngine()
{

}

//*********************************************************************************
//FUNCTION:
bool CGLRenderEngine::_initV()
{
	_HIVE_EARLY_RETURN(!__createGLFWWindow(), "Fail to initialize opengl render engine due to failure of __createGLFWWindow().", false);

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderEngine::_renderV()
{
	glfwPollEvents();

	glfwSwapBuffers(m_pWindow);

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderEngine::_isRenderLoopDoneV()
{
	bool IsRenderLoopDone = glfwWindowShouldClose(m_pWindow);
	if (IsRenderLoopDone) glfwTerminate();

	return IsRenderLoopDone;
}

//*********************************************************************************
//FUNCTION:
bool CGLRenderEngine::__createGLFWWindow()
{
	_HIVE_EARLY_RETURN(!glfwInit(), "Fail to initialize glfw due to failure of glfwInit().", false);

	auto pDisplayDevice = fetchDisplayDevice();
	_ASSERTE(pDisplayDevice);
	auto DisplayInfo = pDisplayDevice->getDisplayDeviceInfo();
	_ASSERTE(DisplayInfo.isValid());

	_ASSERTE(!m_pWindow);
	GLFWmonitor *pMonitor = DisplayInfo.IsFullScreenWindow ? glfwGetPrimaryMonitor() : nullptr;
	m_pWindow = glfwCreateWindow(DisplayInfo.WinWidth, DisplayInfo.WinHeight, DisplayInfo.WinTitle.c_str(), pMonitor, nullptr);
	if (!m_pWindow)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to create window due to failure of glfwCreateWindow()");
		glfwTerminate();
		return false;
	}
	glfwSetWindowPos(m_pWindow, DisplayInfo.WinPosX, DisplayInfo.WinPosY);
	glfwMakeContextCurrent(m_pWindow);
	glewExperimental = GL_TRUE;

	_HIVE_EARLY_RETURN((glewInit() != GLEW_OK), "Fail to initialize glew due to failure of glewInit().", false);

	return true;
}