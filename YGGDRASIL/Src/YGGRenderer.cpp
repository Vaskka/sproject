#include "YGGRenderer.h"
#include "Scene.h"
#include "Model.h"
#include "Camera.h"
#include "YGGShadingTechnique.h"
#include "TextureUtil.h"
#include "RenderUtil.h"
#include "Constants.h"

using namespace Constant;

namespace
{
	bool g_Keys[1024];
	bool g_Buttons[3];
}

CYGGRenderer::CYGGRenderer()
{

}

CYGGRenderer::~CYGGRenderer()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
bool CYGGRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen)
{
	if (!CGLRenderer::initV(vWindowTitle, vWindowWidth, vWindowHeight)) return false;

	__initScene();
	__initTextures();
	__initBuffers();
	m_pShadingTechnique = new CYGGShadingTechnique();
	m_pShadingTechnique->initTechniqueV();
	m_StartTime = clock();

	_registerKeyCallback(__keyCallback);
	_registerCursorPosCallback(__cursorPosCallback);
	_registerMouseButtonCallback(__mouseButtonCallback);
	_registerScrollCallback(__scrollCallback);

	return true;
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::_renderV()
{
	__renderSkyPass();
	__renderGeometryPass();
	__postProcessPass();
	m_CurrentTime = clock();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::_handleEventsV()
{
	float FrameInterval = static_cast<float>(this->getFrameInterval());

	if (g_Keys[GLFW_KEY_W])
		m_pScene->getCamera()->processKeyboard(FORWARD, FrameInterval);
	if (g_Keys[GLFW_KEY_S])
		m_pScene->getCamera()->processKeyboard(BACKWARD, FrameInterval);
	if (g_Keys[GLFW_KEY_A])
		m_pScene->getCamera()->processKeyboard(LEFT, FrameInterval);
	if (g_Keys[GLFW_KEY_D])
		m_pScene->getCamera()->processKeyboard(RIGHT, FrameInterval);
	if (g_Keys[GLFW_KEY_Q])
		m_pScene->getCamera()->processKeyboard(UP, FrameInterval);
	if (g_Keys[GLFW_KEY_E])
		m_pScene->getCamera()->processKeyboard(DOWN, FrameInterval);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__initScene()
{
	m_pScene = CScene::getInstance();
	m_pScene->initScene();
	m_ProjectionMatrix = glm::perspective(m_pScene->getCamera()->Zoom, (float)WIN_WIDTH / (float)WIN_HEIGHT, CAMERA_NEAR, CAMERA_FAR); //NOTE: 必须在initScene之后调用
	//auto pModel = new CModel();
	//pModel->load("res/objects/emeishan/emeishan.obj");
	//m_pScene->addModel(pModel);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__initTextures()
{
	m_WhiteNoiseTex = util::loadTexture(WHITE_NOISE_TEXTURE_PATH.c_str(), GL_REPEAT, GL_LINEAR);
	m_SceneTexture = util::setupTexture(WIN_WIDTH, WIN_HEIGHT);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__initBuffers()
{
	glGenFramebuffers(1, &m_CaptureFBO);
	glGenRenderbuffers(1, &m_CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__renderSkyPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTexture, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	m_pShadingTechnique->enableShader("RenderSkyPass");

	glm::mat4 ViewMatrix = m_pScene->getCamera()->getViewMatrix();
	m_pShadingTechnique->updateStandShaderUniform("uProjectionMatrix", m_ProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uTime", getTime());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WhiteNoiseTex);
	m_pShadingTechnique->updateStandShaderUniform("uWhiteNoiseTex", 0);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	util::renderCube();
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__renderGeometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTexture, 0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	m_pShadingTechnique->enableShader("RenderGeometryPass");

	//update uniforms for PBR
	glm::mat4 ViewMatrix = m_pScene->getCamera()->getViewMatrix();
	glm::mat4 ModelMatrix;
	ModelMatrix = glm::translate(ModelMatrix, MODEL_INIT_TRANSLATE);
	ModelMatrix = glm::scale(ModelMatrix, MODEL_INIT_SCALE);
	m_pShadingTechnique->updateStandShaderUniform("uProjectionMatrix", m_ProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uModelMatrix", ModelMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uEyePositionW", m_pScene->getCamera()->Position);

	for (auto i = 0u; i < m_pScene->getNumModels(); ++i)
	{
		auto pModel = m_pScene->getModelAt(i);
		pModel->draw(m_pShadingTechnique->getProgramID("GeometryPass"));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__postProcessPass()
{
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("PostprocessPass");
	m_pShadingTechnique->updateStandShaderUniform("uSceneTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SceneTexture);
	util::renderScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__destory()
{
	if (m_pScene) m_pScene->destroyScene();
	if (m_pShadingTechnique) delete m_pShadingTechnique;
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode)
{
	if (vKey == GLFW_KEY_ESCAPE && vAction == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, GL_TRUE);
	if (vKey >= 0 && vKey < 1024)
	{
		if (vAction == GLFW_PRESS)
			g_Keys[vKey] = true;
		else if (vAction == GLFW_RELEASE)
			g_Keys[vKey] = false;
	}
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool FirstMouse = true;
	static double LastX = WIN_WIDTH / 2.0;
	static double LastY = WIN_HEIGHT / 2.0;
	if (FirstMouse)
	{
		LastX = xpos;
		LastY = ypos;
		FirstMouse = false;
	}

	double xoffset = xpos - LastX;
	double yoffset = LastY - ypos;

	LastX = xpos;
	LastY = ypos;

	if (g_Buttons[GLFW_MOUSE_BUTTON_LEFT])
		CScene::getInstance()->getCamera()->processMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
{
	if (vButton >= 0 && vButton < 3)
	{
		if (vAction == GLFW_PRESS)
			g_Buttons[vButton] = true;
		else if (vAction == GLFW_RELEASE)
			g_Buttons[vButton] = false;
	}
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	CScene::getInstance()->getCamera()->processMouseScroll(static_cast<float>(vYOffset));
}
