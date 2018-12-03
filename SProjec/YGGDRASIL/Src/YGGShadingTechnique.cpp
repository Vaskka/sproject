#include "YGGShadingTechnique.h"

CYGGShadingTechnique::CYGGShadingTechnique()
{
}

CYGGShadingTechnique::~CYGGShadingTechnique()
{
}

//*********************************************************************************
//FUNCTION:
void CYGGShadingTechnique::initTechniqueV()
{
	CShadingTechnique::initTechniqueV();

	auto pRenderSkyPass = new CShader;
	pRenderSkyPass->addShader("res/shaders/Skybox_VS.glsl", VERTEX_SHADER);
	pRenderSkyPass->addShader("res/shaders/Skybox_FS.glsl", FRAGMENT_SHADER);
	this->addShader("RenderSkyPass", pRenderSkyPass);

	auto pRenderTerrainPass = new CShader;
	pRenderTerrainPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pRenderTerrainPass->addShader("res/shaders/Terrain_FS.glsl", FRAGMENT_SHADER);
	this->addShader("RenderTerrainPass", pRenderTerrainPass);

	auto pRenderGeometryPass = new CShader;
	pRenderGeometryPass->addShader("res/shaders/PBRShading_VS.glsl", VERTEX_SHADER);
	pRenderGeometryPass->addShader("res/shaders/ForwardPBRShading_FS.glsl", FRAGMENT_SHADER);
	this->addShader("RenderGeometryPass", pRenderGeometryPass);

	auto pPostprocessPass = new CShader;
	pPostprocessPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pPostprocessPass->addShader("res/shaders/PostProcess_FS.glsl", FRAGMENT_SHADER);
	this->addShader("PostprocessPass", pPostprocessPass);
}