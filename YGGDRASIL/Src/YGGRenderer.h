#pragma once
#include <string>
#include <GLM/glm.hpp>
#include "GLRenderer.h"

class CCamera;
class CModel;
class CShadingTechnique;
class CScene;

class CYGGRenderer : public CGLRenderer
{
public:
	CYGGRenderer();
	virtual ~CYGGRenderer();

	virtual void initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight) override;

protected:
	virtual void _drawV() override;
	virtual void _handleEventsV() override;

private:
	static CScene *m_pScene;

	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
	GLuint m_SceneTexture;
	GLuint m_WhiteNoiseTex;

	glm::mat4 m_ProjectionMatrix;
	CShadingTechnique *m_pShadingTechnique = nullptr;

	void __initTechniques();
	void __initScene();
	void __initTextures();
	void __initBuffers();
	void __initMatrixs();

	void __envmapPass();
	void __geometryPass();
	void __postProcessPass();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
	static void __scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);
};