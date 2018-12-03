#include "IrrklangAudioPlayer.h"
#include <common/HiveCommonMicro.h>
#include <common/CommonInterface.h>
#include <common/ProductFactory.h>

using namespace sengine::audioEngine;

hiveOO::CProductFactory<CIrrklangAudioPlayer> Creator("IrrklangAudioPlayer");

CIrrklangAudioPlayer::CIrrklangAudioPlayer()
{

}

CIrrklangAudioPlayer::~CIrrklangAudioPlayer()
{
	_destroyV();
}

//*********************************************************************************
//FUNCTION:
bool CIrrklangAudioPlayer::_initV()
{
	m_SoundEngine = irrklang::createIrrKlangDevice();
	_HIVE_EARLY_RETURN(!m_SoundEngine, "Fail to initialize irrklang audio player due to failure of irrklang::createIrrKlangDevice().", false);

	return true;
}

//*********************************************************************************
//FUNCTION:
int CIrrklangAudioPlayer::_playAudio2DV(const std::string& vFilePath)
{
	_ASSERTE(!vFilePath.empty());
	m_SoundEngine->play2D(vFilePath.c_str(), true);

	return 0;
}

//*********************************************************************************
//FUNCTION:
void CIrrklangAudioPlayer::_stopAllAudiosV()
{
	m_SoundEngine->stopAllSounds();
}

//*********************************************************************************
//FUNCTION:
void CIrrklangAudioPlayer::_destroyV()
{
	if (m_SoundEngine)
	{
		m_SoundEngine->drop();
		_SAFE_DELETE(m_SoundEngine);
	}
}