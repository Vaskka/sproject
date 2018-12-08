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
AudioID CIrrklangAudioPlayer::_playAudio2DV(const std::string& vFilePath)
{
	_ASSERTE(!vFilePath.empty());

	auto pSoundSource = m_SoundEngine->addSoundSourceFromFile(vFilePath.c_str(), irrklang::ESM_NO_STREAMING, true);
	pSoundSource->setForcedStreamingThreshold(0);

	auto pSound = m_SoundEngine->play2D(pSoundSource, true, false, true);
	if (!pSound) return -1;

	m_ID2SoundMap[++m_AudioID] = pSound;

	return m_AudioID;
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
		m_SoundEngine->drop(); //NOTE: 这里m_SoundEngine不需要delete
}

//*********************************************************************************
//FUNCTION:
void* sengine::audioEngine::CIrrklangAudioPlayer::_getAudioSampleDataV(AudioID vAudioID) const
{
	const auto Iter = m_ID2SoundMap.find(vAudioID);
	if (Iter == m_ID2SoundMap.end()) return nullptr;

	auto pSound = Iter->second;
	_ASSERTE(pSound);

	auto pSoundSource = pSound->getSoundSource();
	_ASSERTE(pSoundSource);

	return pSoundSource->getSampleData();
}