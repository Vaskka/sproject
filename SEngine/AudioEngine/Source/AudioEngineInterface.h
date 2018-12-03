#pragma once
#include <string>
#include "AudioEngineExport.h"

namespace sengine
{
	namespace audioEngine
	{
		AUDIO_ENGINE_DLL_EXPORT bool init();
		AUDIO_ENGINE_DLL_EXPORT int playAudio2D(const std::string& vFilePath);
		AUDIO_ENGINE_DLL_EXPORT void stopAllAudios();
	}
}