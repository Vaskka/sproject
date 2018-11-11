#pragma once
#include <string>
#include <ctime>
#include <GLM/glm.hpp>
#include "GLRenderer.h"

class CShadingTechnique;
class CScene;

class CYGGRenderer : public CGLRenderer
{
public:
	CYGGRenderer();
	virtual ~CYGGRenderer();

	virtual bool initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen = false) override;

	float getTime() const { return (float)(m_CurrentTime - m_StartTime) / CLOCKS_PER_SEC; }

protected:
	virtual void _renderV() override;
	virtual void _handleEventsV() override;

private:
	GLuint m_CaptureFBO;
	GLuint m_CaptureRBO;
	GLuint m_SceneTexture;
	GLuint m_WhiteNoiseTex;

	glm::mat4 m_ProjectionMatrix;
	CShadingTechnique *m_pShadingTechnique = nullptr;
	CScene *m_pScene = nullptr;

	clock_t m_StartTime = 0;
	clock_t m_CurrentTime = 0;

	void __initScene();
	void __initTextures();
	void __initBuffers();

	void __renderSkyPass();
	void __renderGeometryPass();
	void __postProcessPass();

	void __destory();

	static void __keyCallback(GLFWwindow* vWindow, int vKey, int vScancode, int vAction, int vMode);
	static void __cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void __mouseButtonCallback(GLFWwindow* vWindow, int vButton, int vAction, int vMods);
	static void __scrollCallback(GLFWwindow* vWindow, double vXOffset, double vYOffset);
};