#include "RenderEngineInterface.h"
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include "RenderEngine.h"

using namespace sengine::srenderer;

//*********************************************************************************
//FUNCTION:
bool sengine::srenderer::init()
{
	try
	{
		return CRenderEngine::getInstance()->init();
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to initialize seed rendering engine due to unexpected error.");
		return false;
	}
}

//*********************************************************************************
//FUNCTION:
bool sengine::srenderer::run()
{
	bool IsNormalExit = CRenderEngine::getInstance()->run();
	CRenderEngine::getInstance()->destroy();

	return IsNormalExit;
}

//*********************************************************************************
//FUNCTION:
CDisplayDevice* sengine::srenderer::fetchDisplayDevice()
{
	return CRenderEngine::getInstance()->fetchDisplayDevice();
}