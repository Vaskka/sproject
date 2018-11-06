#include "yggRendering.h"
#include "scene.h"
#include "model.h"
#include "camera.h"
#include "yggShadingTechnique.h"
#include "textureUtil.h"
#include "renderUtil.h"
#include "haltonSampleGenerator.h"
#include "temporalAATechnique.h"
#include "constants.h"

using namespace Constant;

namespace
{
	bool g_Keys[1024];
	bool g_Buttons[3];
}

CScene* CYGGRendering::m_pScene = nullptr;

CYGGRendering::CYGGRendering() : m_pShadingTechnique(nullptr), m_pTAATechnique(nullptr), m_IsUsingTAA(true)
{

}

CYGGRendering::~CYGGRendering()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight)
{
	COpenGLRendering::initV(vWindowTitle, vWindowWidth, vWindowHeight);

	__initScene();
	__initTechniques();
	__initTextures();
	__initMatrixs();
	__initBuffers();

	__equirectangular2CubemapPass();
	__createIrradianceCubemapPass();
	__createBRDFIntegrationTexPass();
	__createPrefilterCubeMapPass();

	glfwSetKeyCallback(m_pWindow, __keyCallback);
	glfwSetCursorPosCallback(m_pWindow, __cursorPosCallback);
	glfwSetMouseButtonCallback(m_pWindow, __mouseButtonCallback);
	glfwSetScrollCallback(m_pWindow, __scrollCallback);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::_drawV()
{
	__geometryPass();
	__envmapPass();
	__temporalAAPass();
	__copyHistoryTexturePass();
	__postProcessPass();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::_handleEventsV()
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
void CYGGRendering::__initTechniques()
{
	m_pShadingTechnique = new CYGGShadingTechnique();
	m_pShadingTechnique->initTechniqueV();

	m_pTAATechnique = new CTAATechnique(new CHaltonSampleGenerator(2, 8), m_ProjectionMatrix, m_pScene->getCamera()->getViewMatrix(), WIN_WIDTH, WIN_HEIGHT);
	m_pTAATechnique->initTechniqueV();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initScene()
{
	m_pScene = CScene::getInstance();
	m_pScene->initScene();

	auto pModel = new CModel();
	pModel->load("res/objects/emeishan/emeishan.obj");
	m_pScene->addModel(pModel);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initTextures()
{
	m_EnvCubemap = util::setupCubemap(EVN_MAP_SIZE, EVN_MAP_SIZE, true);
	m_IrradianceMap = util::setupCubemap(IRRADIANCE_MAP_SIZE, IRRADIANCE_MAP_SIZE, false);
	m_PrefilterMap = util::setupCubemap(PREFILTERED_MAP_SIZE, PREFILTERED_MAP_SIZE, true);

	m_EnvironmentTex = util::loadTexture(EVN_MAP_PATH.c_str());
	m_BRDFIntegrationTex = util::setupTexture(BRDF_INT_MAP_SIZE, BRDF_INT_MAP_SIZE);
	m_CurrentSceneTex = util::setupTexture(WIN_WIDTH, WIN_HEIGHT);
	m_FinalSceneTex = util::setupTexture(WIN_WIDTH, WIN_HEIGHT);
	m_HistorySceneTex = util::setupTexture(WIN_WIDTH, WIN_HEIGHT);
	m_MotionVectorTex = util::setupTexture(WIN_WIDTH, WIN_HEIGHT, GL_RG32F, GL_RG);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initBuffers()
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
void CYGGRendering::__initMatrixs()
{
	m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, CAMERA_NEAR, CAMERA_FAR);
	m_CaptureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	m_CaptureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	m_CaptureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	m_ProjectionMatrix = glm::perspective(m_pScene->getCamera()->Zoom, (float)WIN_WIDTH / (float)WIN_HEIGHT, CAMERA_NEAR, CAMERA_FAR);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__equirectangular2CubemapPass()
{
	m_pShadingTechnique->enableShader("Equirectangular2CubemapPass");
	m_pShadingTechnique->updateTextureShaderUniform("equirectangularMap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_EnvironmentTex);

	m_pShadingTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
	glViewport(0, 0, EVN_MAP_SIZE, EVN_MAP_SIZE);
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);

	unsigned int maxMipLevels = PREFILTER_MAX_MIP_LEVELS;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		unsigned int mipWidth = (unsigned int)(EVN_MAP_SIZE * std::pow(0.5, mip));
		unsigned int mipHeight = (unsigned int)(EVN_MAP_SIZE * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_pShadingTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			util::renderCube();
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pShadingTechnique->disableShader();
	glBindTexture(GL_TEXTURE_2D, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__createIrradianceCubemapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IRRADIANCE_MAP_SIZE, IRRADIANCE_MAP_SIZE);

	m_pShadingTechnique->enableShader("IrradianceCubemapPass");
	m_pShadingTechnique->updateTextureShaderUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);

	m_pShadingTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
	glViewport(0, 0, IRRADIANCE_MAP_SIZE, IRRADIANCE_MAP_SIZE);

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_pShadingTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		util::renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__createBRDFIntegrationTexPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, BRDF_INT_MAP_SIZE, BRDF_INT_MAP_SIZE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFIntegrationTex, 0);

	glViewport(0, 0, BRDF_INT_MAP_SIZE, BRDF_INT_MAP_SIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pShadingTechnique->enableShader("BRDFIntegrationPass");
	util::renderScreenQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__createPrefilterCubeMapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, PREFILTERED_MAP_SIZE, PREFILTERED_MAP_SIZE);

	m_pShadingTechnique->enableShader("PrefilterCubemapPass");
	m_pShadingTechnique->updateTextureShaderUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);
	m_pShadingTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
	glViewport(0, 0, PREFILTERED_MAP_SIZE, PREFILTERED_MAP_SIZE);

	unsigned int maxMipLevels = PREFILTER_MAX_MIP_LEVELS;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = (unsigned int)(PREFILTERED_MAP_SIZE * std::pow(0.5, mip));
		unsigned int mipHeight = (unsigned int)(PREFILTERED_MAP_SIZE * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		m_pShadingTechnique->updateStandShaderUniform("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_pShadingTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			util::renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__envmapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_CurrentSceneTex, 0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glDepthFunc(GL_LEQUAL);
	m_pShadingTechnique->enableShader("EvnMapPass");

	glm::mat4 ProjectionMatrix;
	m_pTAATechnique->undoProjectionOffset(ProjectionMatrix, m_ProjectionMatrix);
	glm::mat4 ViewMatrix = m_pScene->getCamera()->getViewMatrix();
	m_pShadingTechnique->updateStandShaderUniform("uProjectionMatrix", ProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uEnvmap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);
	util::renderCube();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
	glDepthFunc(GL_LESS);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__geometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_CurrentSceneTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_MotionVectorTex, 0);
	GLuint Attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, Attachments);

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
	m_pShadingTechnique->updateStandShaderUniform("uIrradianceMap", 5);
	m_pShadingTechnique->updateStandShaderUniform("uPrefilteredMap", 6);
	m_pShadingTechnique->updateStandShaderUniform("uBrdfIntegrationMap", 7);

	//update uniforms for TAA
	m_pTAATechnique->update(m_ProjectionMatrix, m_pScene->getCamera()->getViewMatrix());
	m_ProjectionMatrix = m_pTAATechnique->getCurrentCameraInfo().ProjectionMatrix;

	SCameraMatrixInfo& CurrentCameraMatrixInfo = m_pTAATechnique->getCurrentCameraInfo();
	SCameraMatrixInfo& HistoryCameraMatrixInfo = m_pTAATechnique->getLastCameraInfo();
	glm::mat4 HistoryProjectionMatrix;
	m_pTAATechnique->undoProjectionOffset(HistoryProjectionMatrix, HistoryCameraMatrixInfo.ProjectionMatrix);
	glm::mat4 CurrentProjectionMatrix;
	m_pTAATechnique->undoProjectionOffset(CurrentProjectionMatrix, CurrentCameraMatrixInfo.ProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uHistoryProjectionMatrix", HistoryProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uHistoryViewMatrix", HistoryCameraMatrixInfo.ViewMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uCurrentProjectionMatrix", CurrentProjectionMatrix);
	m_pShadingTechnique->updateStandShaderUniform("uUseTAA", (float)m_IsUsingTAA);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_BRDFIntegrationTex);
	for (int i = 0; i < m_pScene->getNumModels(); ++i)
	{
		auto pModel = m_pScene->getModelAt(i);
		pModel->draw(m_pShadingTechnique->getProgramID("GeometryPass"));
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
	glDrawBuffers(1, Attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__temporalAAPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FinalSceneTex, 0);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("TemporalAntiAliasingPass");
	m_pShadingTechnique->updateStandShaderUniform("uCurrentSceneTexture", 0);
	m_pShadingTechnique->updateStandShaderUniform("uHistorySceneTexture", 1);
	m_pShadingTechnique->updateStandShaderUniform("uMotionVectorTexture", 2);
	m_pShadingTechnique->updateStandShaderUniform("uUseTAA", (float)m_IsUsingTAA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_CurrentSceneTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_HistorySceneTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_MotionVectorTex);
	util::renderScreenQuad();

	m_pShadingTechnique->disableShader();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__copyHistoryTexturePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WIN_WIDTH, WIN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HistorySceneTex, 0);
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("HistoryTextureCopyPass");
	m_pShadingTechnique->updateStandShaderUniform("uSceneTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FinalSceneTex);
	util::renderScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	m_pShadingTechnique->disableShader();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__postProcessPass()
{
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pShadingTechnique->enableShader("PostprocessPass");
	m_pShadingTechnique->updateStandShaderUniform("uSceneTexture", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FinalSceneTex);
	util::renderScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	m_pShadingTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__destory()
{
	if (m_pScene) m_pScene->destroyScene();
	if (m_pShadingTechnique) delete m_pShadingTechnique;
	if (m_pTAATechnique) delete m_pTAATechnique;
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode)
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
void CYGGRendering::__cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	static bool FirstMouse = true;
	static GLfloat LastX = WIN_WIDTH / 2.0;
	static GLfloat LastY = WIN_HEIGHT / 2.0;
	if (FirstMouse)
	{
		LastX = xpos;
		LastY = ypos;
		FirstMouse = false;
	}

	GLfloat xoffset = xpos - LastX;
	GLfloat yoffset = LastY - ypos;

	LastX = xpos;
	LastY = ypos;

	if (g_Buttons[GLFW_MOUSE_BUTTON_LEFT])
		m_pScene->getCamera()->processMouseMovement(xoffset, yoffset);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods)
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
void CYGGRendering::__scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	m_pScene->getCamera()->processMouseScroll(vYOffset);
}
