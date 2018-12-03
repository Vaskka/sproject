#pragma once
#include <string>
#include <common/Singleton.h>
#include "AudioEngineExport.h"

namespace sengine
{
	namespace audioEngine
	{
		class IAudioEngineImp;

		class CAudioEngine : public hiveOO::CSingleton<CAudioEngine>
		{
		public:
			CAudioEngine();
			virtual ~CAudioEngine();

			bool init();
			void destroy();

			int playAudio2D(const std::string& vFilePath);
			void stopAllAudios() const;

		private:
			bool m_IsInitialized = false;
			IAudioEngineImp* m_pAudioEngineImp = nullptr;

			friend class hiveOO::CSingleton<CAudioEngine>;
		};
	}
}