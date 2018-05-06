#include "yggRendering.h"
#include "model.h"
#include "camera.h"
#include "yggTechnique.h"
#include "textureUtil.h"
#include "renderUtil.h"

namespace
{
	int SCR_WIDTH = 1920;
	int SCR_HEIGHT = 1080;
	bool g_Keys[1024];
	bool g_Buttons[3];
	CCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	const std::string EVN_MAP_PATH = "res/textures/UnderTheSea4k.hdr";
	const std::string GEOMERY_MODEL_PATH = "res/objects/miku/miku_snow/miku.obj";
	const std::string CUBE_MODEL_PATH = "res/objects/cube.obj";
	const unsigned int EVN_MAP_SIZE = 1024;
	const unsigned int IRRADIANCE_MAP_SIZE = 128;
	const unsigned int PREFILTERED_MAP_SIZE = 1024;
	const unsigned int BRDF_INT_MAP_SIZE = 1024;
	const unsigned int PREFILTER_MAX_MIP_LEVELS = 6;
}

CYGGRendering::CYGGRendering() : m_pWindow(nullptr), m_pModel(nullptr), m_pTechnique(nullptr)
{
}

CYGGRendering::~CYGGRendering()
{
	__destory();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::init()
{
	__initGLFW("YGGDRASIL");
	__initShaders();
	__initModels();
	__initTextures();
	__initMatrixs();
	__initBuffers();

	__equirectangular2CubemapPass();
	__createIrradianceCubemapPass();
	__createBRDFIntegrationTexPass();
	__createPrefilterCubeMapPass();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::draw()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		GLfloat CurrentFrame = glfwGetTime();
		m_DeltaTime = CurrentFrame - m_LastFrame;
		m_LastFrame = CurrentFrame;

		glfwPollEvents();
		__doMovement();

		__geometryPass();
		__envmapPass();

		glfwSwapBuffers(m_pWindow);
	}
	glfwTerminate();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initGLFW(const std::string& vWindowTitle)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_pWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, vWindowTitle.c_str(), nullptr, nullptr);
	_ASSERTE(m_pWindow);
	glfwMakeContextCurrent(m_pWindow);

	glfwSetKeyCallback(m_pWindow, __keyCallback);
	glfwSetCursorPosCallback(m_pWindow, __cursorPosCallback);
	glfwSetMouseButtonCallback(m_pWindow, __mouseButtonCallback);
	glfwSetScrollCallback(m_pWindow, __scrollCallback);
	glewExperimental = GL_TRUE;

	glewInit();
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initShaders()
{
	m_pTechnique = new CYGGTechnique();
	m_pTechnique->initTechniqueV();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initModels()
{
	m_pModel = new CModel();
	m_pModel->load(GEOMERY_MODEL_PATH);

	m_pEvnCube = new CModel();
	m_pEvnCube->load(CUBE_MODEL_PATH);
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
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initBuffers()
{
	glGenFramebuffers(1, &m_CaptureFBO);
	glGenRenderbuffers(1, &m_CaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, EVN_MAP_SIZE, EVN_MAP_SIZE);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_CaptureRBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__initMatrixs()
{
	m_CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	m_CaptureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	m_CaptureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	m_CaptureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	m_CaptureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__equirectangular2CubemapPass()
{
	m_pTechnique->enableShader("Equirectangular2CubemapPass");
	m_pTechnique->updateTextureShaderUniform("equirectangularMap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_EnvironmentTex);

	m_pTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
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
			m_pTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubemap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_pEvnCube->draw(m_pTechnique->getProgramID("Equirectangular2CubemapPass"));
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_pTechnique->disableShader();
	glBindTexture(GL_TEXTURE_2D, 0);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__createIrradianceCubemapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IRRADIANCE_MAP_SIZE, IRRADIANCE_MAP_SIZE);

	m_pTechnique->enableShader("IrradianceCubemapPass");
	m_pTechnique->updateTextureShaderUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);

	m_pTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
	glViewport(0, 0, IRRADIANCE_MAP_SIZE, IRRADIANCE_MAP_SIZE);

	for (unsigned int i = 0; i < 6; ++i)
	{
		m_pTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pEvnCube->draw(m_pTechnique->getProgramID("IrradianceCubemapPass"));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_pTechnique->disableShader();
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
	m_pTechnique->enableShader("BRDFIntegrationPass");
	util::renderScreenQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	m_pTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__createPrefilterCubeMapPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_CaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_CaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, PREFILTERED_MAP_SIZE, PREFILTERED_MAP_SIZE);

	m_pTechnique->enableShader("PrefilterCubemapPass");
	m_pTechnique->updateTextureShaderUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);
	m_pTechnique->updateStandShaderUniform("projection", m_CaptureProjection);
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
		m_pTechnique->updateStandShaderUniform("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			m_pTechnique->updateStandShaderUniform("view", m_CaptureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_pEvnCube->draw(m_pTechnique->getProgramID("PrefilterCubemapPass"));
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	m_pTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__envmapPass()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glDepthFunc(GL_LEQUAL);
	m_pTechnique->enableShader("EvnMapPass");

	glm::mat4 ViewMatrix = g_Camera.getViewMatrix();
	m_pTechnique->updateStandShaderUniform("uProjectionMatrix", m_ProjectionMatrix);
	m_pTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pTechnique->updateStandShaderUniform("uEnvmap", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubemap);
	m_pEvnCube->draw(m_pTechnique->getProgramID("EvnMapPass"));

	m_pTechnique->disableShader();
	glDepthFunc(GL_LESS);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__geometryPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pTechnique->enableShader("GeometryPass");

	glm::mat4 ViewMatrix = g_Camera.getViewMatrix();
	glm::mat4 ModelMatrix;
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.5f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.02f, 0.02f, 0.02f));
	m_ProjectionMatrix = glm::perspective(g_Camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	m_pTechnique->updateStandShaderUniform("uProjectionMatrix", m_ProjectionMatrix);
	m_pTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pTechnique->updateStandShaderUniform("uModelMatrix", ModelMatrix);
	m_pTechnique->updateStandShaderUniform("uEyePositionW", g_Camera.Position);
	m_pTechnique->updateStandShaderUniform("uIrradianceMap", 5);
	m_pTechnique->updateStandShaderUniform("uPrefilteredMap", 6);
	m_pTechnique->updateStandShaderUniform("uBrdfIntegrationMap", 7);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, m_BRDFIntegrationTex);
	m_pModel->draw(m_pTechnique->getProgramID("GeometryPass"));

	m_pTechnique->disableShader();
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__doMovement()
{
	if (g_Keys[GLFW_KEY_W])
		g_Camera.processKeyboard(FORWARD, m_DeltaTime);
	if (g_Keys[GLFW_KEY_S])
		g_Camera.processKeyboard(BACKWARD, m_DeltaTime);
	if (g_Keys[GLFW_KEY_A])
		g_Camera.processKeyboard(LEFT, m_DeltaTime);
	if (g_Keys[GLFW_KEY_D])
		g_Camera.processKeyboard(RIGHT, m_DeltaTime);
	if (g_Keys[GLFW_KEY_Q])
		g_Camera.processKeyboard(UP, m_DeltaTime);
	if (g_Keys[GLFW_KEY_E])
		g_Camera.processKeyboard(DOWN, m_DeltaTime);
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__destory()
{
	if (m_pModel) delete m_pModel;
	if (m_pEvnCube) delete m_pEvnCube;
	if (m_pTechnique) delete m_pTechnique;
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
	static GLfloat LastX = SCR_WIDTH / 2.0;
	static GLfloat LastY = SCR_HEIGHT / 2.0;
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
		g_Camera.processMouseMovement(xoffset, yoffset);
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
	g_Camera.processMouseScroll(vYOffset);
}
