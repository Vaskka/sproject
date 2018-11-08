#pragma once

#include <string>
#include <GLM/glm.hpp>
#include "openglRendering.h"

class CCamera;
class CModel;
class CShadingTechnique;
class CTAATechnique;
class CScene;

class CYGGRendering : public COpenGLRendering
{
public:
	CYGGRendering();
	virtual ~CYGGRendering();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight) override;

protected:
	virtual void _drawV() override;
	virtual void _handleEventsV() override;

private:
	void __initTechniques();
	void __initScene();
	void __initTextures();
	void __initBuffers();
	void __initMatrixs();

	void __equirectangular2CubemapPass();
	void __createIrradianceCubemapPass();
	void __createBRDFIntegrationTexPass();
	void __createPrefilterCubeMapPass();
	void __envmapPass();
	void __geometryPass();
	void __temporalAAPass();
	void __copyHistoryTexturePass();
	void __postProcessPass();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
	static void __scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);

private:
	static CScene *m_pScene;

	CShadingTechnique *m_pShadingTechnique = nullptr;
	CTAATechnique *m_pTAATechnique = nullptr;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;

	GLuint m_EnvironmentTex;
	GLuint m_BRDFIntegrationTex;
	GLuint m_CurrentSceneTex;
	GLuint m_FinalSceneTex;
	GLuint m_HistorySceneTex;
	GLuint m_MotionVectorTex;
	GLuint m_WhiteNoiseTex;
	GLuint m_EnvCubemap;
	GLuint m_IrradianceMap;
	GLuint m_PrefilterMap;

	glm::mat4 m_CaptureProjection;
	glm::mat4 m_CaptureViews[6];
	glm::mat4 m_ProjectionMatrix;

	bool m_IsUsingTAA = false;
};