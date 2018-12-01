#pragma once
#include <irrKlang.h>
#include "common/Singleton.h"

class CSoundPlayer : public hiveOO::CSingleton<CSoundPlayer>
{
public:
	CSoundPlayer();
	virtual ~CSoundPlayer();

	bool init();
	void playSound2D(const char* vFilePath);
	void stopAllSounds() const;

private:
	bool m_IsInitialized = false;
	irrklang::ISoundEngine* m_SoundEngine = nullptr;

	friend class hiveOO::CSingleton<CSoundPlayer>;
};