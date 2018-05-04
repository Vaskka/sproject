#include "yggRendering.h"
#include "model.h"
#include "camera.h"
#include "yggTechnique.h"

namespace
{
	int SCR_WIDTH = 1600;
	int SCR_HEIGHT = 900;
	bool g_Keys[1024];
	bool g_Buttons[3];
	CCamera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	m_pModel->load("res/objects/nanosuit/nanosuit.obj");
}

//*********************************************************************************
//FUNCTION:
void CYGGRendering::__geometryPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pTechnique->enableShader("PerPixelShadingPass");

	glm::mat4 ProjectionMatrix = glm::perspective(g_Camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 ViewMatrix = g_Camera.getViewMatrix();
	glm::mat4 ModelMatrix;
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.5f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	m_pTechnique->updateStandShaderUniform("uProjectionMatrix", ProjectionMatrix);
	m_pTechnique->updateStandShaderUniform("uViewMatrix", ViewMatrix);
	m_pTechnique->updateStandShaderUniform("uModelMatrix", ModelMatrix);
	m_pTechnique->updateStandShaderUniform("uViewPos", g_Camera.Position);

	m_pModel->draw(m_pTechnique->getProgramID("PerPixelShadingPass"));

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
