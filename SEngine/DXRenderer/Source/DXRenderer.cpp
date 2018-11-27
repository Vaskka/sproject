#include "DXRenderer.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>
#include "RenderEngineInterface.h"
#include "DisplayDevice.h"

using namespace sengine::renderEngine;

#ifndef _USE_CUSTOM_RENDERER //TODO: bad design
hiveOO::CProductFactory<CDXRenderer> Creator("DXRenderer");
#endif

CDXRenderer::CDXRenderer()
{

}

CDXRenderer::~CDXRenderer()
{

}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_initV()
{
	_HIVE_EARLY_RETURN(!__createDXWindow(), "Fail to initialize directX render engine due to failure of __createDXWindow().", false);

	hiveCommon::hiveOutputEvent("Succeed to init directX renderer.");

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_renderV()
{
	_handleEventV();

	return true;
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::_isRenderLoopDoneV()
{
	return false;
}

//*********************************************************************************
//FUNCTION:
bool CDXRenderer::__createDXWindow()
{
	return true;
}

//*********************************************************************************
//FUNCTION:
void CDXRenderer::_handleEventV()
{

}