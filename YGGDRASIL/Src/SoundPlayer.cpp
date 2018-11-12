#include "SoundPlayer.h"

#ifdef _WINDOWS
#include <windows.h>
#include <mmsystem.h>
#endif

#include "common/HiveCommonMicro.h"
#include "common/CommonInterface.h"

//*********************************************************************************
//FUNCTION:
void CSoundPlayer::playSound(const char* vFilePath) const
{
	_ASSERTE(vFilePath);

#ifdef _WINDOWS
	PlaySound(vFilePath, nullptr, SND_ASYNC | SND_LOOP);
#else
	hiveCommon::hiveOutputWarning(__EXCEPTION_SITE__, "Fail to play sounds because the platform is not supported.");
#endif
}

//*********************************************************************************
//FUNCTION:
void CSoundPlayer::stopAllSounds() const
{
#ifdef _WINDOWS
	PlaySound(nullptr, nullptr, SND_ASYNC | SND_LOOP);
#else
	_ASSERTE(false);
#endif
}