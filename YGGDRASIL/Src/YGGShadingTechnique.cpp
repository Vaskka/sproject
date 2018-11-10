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

	auto pEvnMapPass = new CShader;
	pEvnMapPass->addShader("res/shaders/Skybox_VS.glsl", VERTEX_SHADER);
	pEvnMapPass->addShader("res/shaders/Skybox_FS.glsl", FRAGMENT_SHADER);
	this->addShader("EvnMapPass", pEvnMapPass);

	auto pGeometryPass = new CShader;
	pGeometryPass->addShader("res/shaders/PBRShading_VS.glsl", VERTEX_SHADER);
	pGeometryPass->addShader("res/shaders/ForwardPBRShading_FS.glsl", FRAGMENT_SHADER);
	this->addShader("GeometryPass", pGeometryPass);

	auto pPostprocessPass = new CShader;
	pPostprocessPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pPostprocessPass->addShader("res/shaders/PostProcess_FS.glsl", FRAGMENT_SHADER);
	this->addShader("PostprocessPass", pPostprocessPass);
}