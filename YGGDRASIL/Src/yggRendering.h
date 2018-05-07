#pragma once

#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

class CCamera;
class CModel;
class CYGGTechnique;
class CTemporalAntialiasing;

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
	void __initTextures();
	void __initBuffers();
	void __initMatrixs();
	void __initTemporalAntialiasing();

	void __equirectangular2CubemapPass();
	void __createIrradianceCubemapPass();
	void __createBRDFIntegrationTexPass();
	void __createPrefilterCubeMapPass();
	void __envmapPass();
	void __geometryPass();
	void __temporalAAPass();
	void __copyHistoryTexturePass();
	void __postProcessPass();

	void __handleKeyEvent();
	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
	static void __scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);

private:
	GLFWwindow *m_pWindow;
	CModel *m_pModel;
	CModel *m_pEvnCube;
	CYGGTechnique *m_pRenderingTechnique;
	CTemporalAntialiasing *m_pTemporalAntialiasingComp;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;

	GLuint m_EnvironmentTex;
	GLuint m_BRDFIntegrationTex;
	GLuint m_CurrentSceneTex;
	GLuint m_FinalSceneTex;
	GLuint m_HistorySceneTex;
	GLuint m_MotionVectorTex;
	GLuint m_EnvCubemap;
	GLuint m_IrradianceMap;
	GLuint m_PrefilterMap;

	glm::mat4 m_CaptureProjection;
	glm::mat4 m_CaptureViews[6];
	glm::mat4 m_ProjectionMatrix;

	GLfloat m_DeltaTime = 0.0f;
	GLfloat m_LastFrame = 0.0f;

	bool m_IsUsingTAA;
};