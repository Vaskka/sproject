#pragma once
#include <string>
#include "AudioEngineExport.h"
#include "AudioEngineCommon.h"

namespace sengine
{
	namespace audioEngine
	{
		AUDIO_ENGINE_DLL_EXPORT bool init();
		AUDIO_ENGINE_DLL_EXPORT void destroy();

		AUDIO_ENGINE_DLL_EXPORT AudioID playAudio2D(const std::string& vFilePath);
		AUDIO_ENGINE_DLL_EXPORT void stopAllAudios();
		AUDIO_ENGINE_DLL_EXPORT void* getAudioSampleData(AudioID vAudioID);
	}
}