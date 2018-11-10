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

CScene* CYGGRenderer::m_pScene = nullptr;

CYGGRenderer::CYGGRenderer()
{

}

CYGGRenderer::~CYGGRenderer()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight)
{
	CGLRenderer::initV(vWindowTitle, vWindowWidth, vWindowHeight);

	__initScene();
	__initTechniques();
	__initTextures();
	__initMatrixs();
	__initBuffers();

	glfwSetKeyCallback(m_pWindow, __keyCallback);
	glfwSetCursorPosCallback(m_pWindow, __cursorPosCallback);
	glfwSetMouseButtonCallback(m_pWindow, __mouseButtonCallback);
	glfwSetScrollCallback(m_pWindow, __scrollCallback);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::_drawV()
{
	__geometryPass();
	__envmapPass();
	__postProcessPass();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::_handleEventsV()
{
	if (g_Keys[GLFW_KEY_W])
		m_pScene->getCamera()->processKeyboard(FORWARD, m_DeltaTime);
	if (g_Keys[GLFW_KEY_S])
		m_pScene->getCamera()->processKeyboard(BACKWARD, m_DeltaTime);
	if (g_Keys[GLFW_KEY_A])
		m_pScene->getCamera()->processKeyboard(LEFT, m_DeltaTime);
	if (g_Keys[GLFW_KEY_D])
		m_pScene->getCamera()->processKeyboard(RIGHT, m_DeltaTime);
	if (g_Keys[GLFW_KEY_Q])
		m_pScene->getCamera()->processKeyboard(UP, m_DeltaTime);
	if (g_Keys[GLFW_KEY_E])
		m_pScene->getCamera()->processKeyboard(DOWN, m_DeltaTime);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__initTechniques()
{
	m_pShadingTechnique = new CYGGShadingTechnique();
	m_pShadingTechnique->initTechniqueV();
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__initScene()
{
	m_pScene = CScene::getInstance();
	m_pScene->initScene();

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
void CYGGRenderer::__initMatrixs()
{
	m_ProjectionMatrix = glm::perspective(m_pScene->getCamera()->Zoom, (float)WIN_WIDTH / (float)WIN_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__envmapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTexture, 0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glDepthFunc(GL_LEQUAL);
	m_pShadingTechnique->enableShader("EvnMapPass");

	glm::mat4 ViewMatrix = m_pScene->getCamera()->getViewMatrix();
	m_pShadingTechnique->updateStandShaderUniform("uProjectionMatrix", m_ProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WhiteNoiseTex);
	m_pShadingTechnique->updateStandShaderUniform("uWhiteNoiseTex", 0);

	util::renderCube();

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
	glDepthFunc(GL_LESS);
}

//*********************************************************************************
//FUNCTION:
void CYGGRenderer::__geometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SceneTexture, 0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShadingTechnique->enableShader("GeometryPass");

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
		m_pScene->getCamera()->processMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
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
	m_pScene->getCamera()->processMouseScroll(static_cast<float>(vYOffset));
}