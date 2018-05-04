#include "yggTechnique.h"

CYGGTechnique::CYGGTechnique()
{
}

CYGGTechnique::~CYGGTechnique()
{
}

//*********************************************************************************
//FUNCTION:
bool CYGGTechnique::initTechniqueV()
{
	auto pPerPixelShadingPass = new CShader;
	pPerPixelShadingPass->addShader("res/shaders/vertPerpixelShading.glsl", VERTEX_SHADER);
	pPerPixelShadingPass->addShader("res/shaders/fragPerpixelShading.glsl", FRAGMENT_SHADER);
	addShader("PerPixelShadingPass", pPerPixelShadingPass);

	return true;
}