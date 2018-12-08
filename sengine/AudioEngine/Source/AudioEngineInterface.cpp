#include "AudioEngineInterface.h"
#include <common/CommonInterface.h>
#include <common/HiveCommonMicro.h>
#include "AudioEngine.h"

using namespace sengine::audioEngine;

//*********************************************************************************
//FUNCTION:
AUDIO_ENGINE_DLL_EXPORT bool sengine::audioEngine::init()
{
	try
	{
		return CAudioEngine::getInstance()->init();
	}
	catch (...)
	{
		hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to initialize audio engine due to unexpected error.");
		return false;
	}
}

//*********************************************************************************
//FUNCTION:
AUDIO_ENGINE_DLL_EXPORT void sengine::audioEngine::destroy()
{
	CAudioEngine::getInstance()->destroy();
}

//*********************************************************************************
//FUNCTION:
AUDIO_ENGINE_DLL_EXPORT AudioID sengine::audioEngine::playAudio2D(const std::string& vFilePath)
{
	return CAudioEngine::getInstance()->playAudio2D(vFilePath);
}

//*********************************************************************************
//FUNCTION:
AUDIO_ENGINE_DLL_EXPORT void sengine::audioEngine::stopAllAudios()
{
	CAudioEngine::getInstance()->stopAllAudios();
}

//*********************************************************************************
//FUNCTION:
AUDIO_ENGINE_DLL_EXPORT void* sengine::audioEngine::getAudioSampleData(AudioID vAudioID)
{
	return CAudioEngine::getInstance()->getAudioSampleData(vAudioID);
}