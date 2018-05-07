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
	auto pEquirectangular2CubemapPass = new CShader;
	pEquirectangular2CubemapPass->addShader("res/shaders/Cubemap_VS.glsl", VERTEX_SHADER);
	pEquirectangular2CubemapPass->addShader("res/shaders/EquirectangularToCubemap_FS.glsl", FRAGMENT_SHADER);
	addShader("Equirectangular2CubemapPass", pEquirectangular2CubemapPass);

	auto pIrradianceCubemapPass = new CShader;
	pIrradianceCubemapPass->addShader("res/shaders/Cubemap_VS.glsl", VERTEX_SHADER);
	pIrradianceCubemapPass->addShader("res/shaders/IrradianceConvolution_FS.glsl", FRAGMENT_SHADER);
	addShader("IrradianceCubemapPass", pIrradianceCubemapPass);

	auto pPrefilterCubemapPass = new CShader;
	pPrefilterCubemapPass->addShader("res/shaders/Cubemap_VS.glsl", VERTEX_SHADER);
	pPrefilterCubemapPass->addShader("res/shaders/Prefilter_FS.glsl", FRAGMENT_SHADER);
	addShader("PrefilterCubemapPass", pPrefilterCubemapPass);

	auto pBRDFIntegrationPass = new CShader;
	pBRDFIntegrationPass->addShader("res/shaders/BRDF_VS.glsl", VERTEX_SHADER);
	pBRDFIntegrationPass->addShader("res/shaders/BRDF_FS.glsl", FRAGMENT_SHADER);
	addShader("BRDFIntegrationPass", pBRDFIntegrationPass);

	auto pEvnMapPass = new CShader;
	pEvnMapPass->addShader("res/shaders/Skybox_VS.glsl", VERTEX_SHADER);
	pEvnMapPass->addShader("res/shaders/Skybox_FS.glsl", FRAGMENT_SHADER);
	addShader("EvnMapPass", pEvnMapPass);

	auto pGeometryPass = new CShader;
	pGeometryPass->addShader("res/shaders/PBRShading_VS.glsl", VERTEX_SHADER);
	pGeometryPass->addShader("res/shaders/ForwardPBRShading_FS.glsl", FRAGMENT_SHADER);
	addShader("GeometryPass", pGeometryPass);

	auto pTemporalAAPass = new CShader;
	pTemporalAAPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pTemporalAAPass->addShader("res/shaders/TemporalAntialiasing_FS.glsl", FRAGMENT_SHADER);
	addShader("TemporalAntiAliasingPass", pTemporalAAPass);

	auto pHistoryTextureCopyPass = new CShader;
	pHistoryTextureCopyPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pHistoryTextureCopyPass->addShader("res/shaders/TextureCopy_FS.glsl", FRAGMENT_SHADER);
	addShader("HistoryTextureCopyPass", pHistoryTextureCopyPass);

	auto pPostprocessPass = new CShader;
	pPostprocessPass->addShader("res/shaders/DrawScreenQuad_VS.glsl", VERTEX_SHADER);
	pPostprocessPass->addShader("res/shaders/PostProcess_FS.glsl", FRAGMENT_SHADER);
	addShader("PostprocessPass", pPostprocessPass);

	return true;
}