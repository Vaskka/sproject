#pragma once
#include <irrKlang.h>
#include "IAudioEngineImp.h"

namespace sengine
{
	namespace audioEngine
	{
		class CIrrklangAudioPlayer : public IAudioEngineImp
		{
		public:
			CIrrklangAudioPlayer();
			virtual ~CIrrklangAudioPlayer();

		protected:
			virtual bool _initV() override;
			virtual int _playAudio2DV(const std::string& vFilePath) override;
			virtual void _stopAllAudiosV() override;
			virtual void _destroyV() override;

		private:
			irrklang::ISoundEngine* m_SoundEngine = nullptr;
		};
	}
}