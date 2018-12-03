#pragma once
#include <common/BaseProduct.h>
#include "AudioEngineExport.h"

namespace sengine
{
	namespace audioEngine
	{
		class AUDIO_ENGINE_DLL_EXPORT IAudioEngineImp : public hiveOO::CBaseProduct
		{
			friend class CAudioEngine;

		public:
			virtual ~IAudioEngineImp() = default;

		protected:
			IAudioEngineImp() = default;

			virtual bool _initV() = 0;
			virtual int _playAudio2DV(const std::string& vFilePath) = 0;
			virtual void _stopAllAudiosV() = 0;
			virtual void _destroyV() = 0;
		};
	}
}