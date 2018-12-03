#include "stdafx.h"
#include "CppUnitTest.h"
#include "AudioEngine.h"
#include "IrrklangAudioPlayer.h"
#include <thread>
#include <common/ProductFactory.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sengine::audioEngine;

hiveOO::CProductFactory<CIrrklangAudioPlayer> Creator1("IrrklangAudioPlayer");

namespace UnitTest_001
{
	TEST_CLASS(TestPlayAudio2D)
	{
	public:
		TEST_METHOD(initAudioEngine)
		{
			bool Success = CAudioEngine::getInstance()->init();
			Assert::IsTrue(Success);
			CAudioEngine::getInstance()->destroy();
		}

		TEST_METHOD(playAudio2D)
		{
			Assert::IsTrue(CAudioEngine::getInstance()->init());
			CAudioEngine::getInstance()->playAudio2D("res/sounds/01.mp3");
			std::this_thread::sleep_for(std::chrono::seconds(10));
			CAudioEngine::getInstance()->stopAllAudios();
		}

		TEST_METHOD(playAudio2DWithoutInit)
		{

		}
	};
}