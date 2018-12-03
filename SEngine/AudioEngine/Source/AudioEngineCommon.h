#pragma once
#include <string>
#include <boost/tuple/tuple.hpp>

namespace sengine
{
	namespace audioEngine
	{
		namespace CONFIG_KEYWORD
		{
			const std::string AUDIO_ENGINE_SIGNATURE = "AUDIO_ENGINE_SIGNATURE";
		}

		namespace CONFIG_DEFAULT
		{
			const std::string DEFAULT_AUDIO_ENGINE_SIGNATURE = "IrrklangAudioPlayer";
		}

		using AudioID = int;
	}
}