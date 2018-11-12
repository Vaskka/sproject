#pragma once
#include "common/Singleton.h"

class CSoundPlayer : public hiveOO::CSingleton<CSoundPlayer>
{
public:
	virtual ~CSoundPlayer() = default;

	void playSound(const char* vFilePath) const;
	void stopAllSounds() const;

private:
	CSoundPlayer() = default;

	friend class hiveOO::CSingleton<CSoundPlayer>;
};