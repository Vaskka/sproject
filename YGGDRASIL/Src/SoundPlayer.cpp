#include "SoundPlayer.h"
#include "common/HiveCommonMicro.h"
#include "common/CommonInterface.h"

CSoundPlayer::CSoundPlayer()
{

}

CSoundPlayer::~CSoundPlayer()
{
	if (m_SoundEngine)
		m_SoundEngine->drop();
}

//*********************************************************************************
//FUNCTION:
bool CSoundPlayer::init()
{
	if (m_IsInitialized) return true;
	m_SoundEngine = irrklang::createIrrKlangDevice();
	_HIVE_EARLY_RETURN(!m_SoundEngine, "Fail to initialize sound player due to failure of irrklang::createIrrKlangDevice().", false);

	m_IsInitialized = true;
	return true;
}

//*********************************************************************************
//FUNCTION:
void CSoundPlayer::playSound2D(const char* vFilePath)
{
	_ASSERTE(vFilePath);
	if (!m_IsInitialized) init();
	m_SoundEngine->play2D(vFilePath, true);
}

//*********************************************************************************
//FUNCTION:
void CSoundPlayer::stopAllSounds() const
{
	if (!m_IsInitialized) return;
	m_SoundEngine->stopAllSounds();
}