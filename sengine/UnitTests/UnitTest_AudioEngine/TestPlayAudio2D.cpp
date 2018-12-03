#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sengine::audioEngine;

static hiveOO::CProductFactory<CIrrklangAudioPlayer> Creator("IrrklangAudioPlayer");

namespace unitTest4AudioEngine
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

		TEST_METHOD(destroyAudioEngine)
		{
			CAudioEngine::getInstance()->destroy();
			Assert::IsTrue(CAudioEngine::getInstance()->init());
			Assert::IsTrue(CAudioEngine::getInstance()->init());
			CAudioEngine::getInstance()->destroy();
			Assert::IsTrue(CAudioEngine::getInstance()->init());
			CAudioEngine::getInstance()->destroy();
		}

		TEST_METHOD(playAudio2D)
		{
			Assert::IsTrue(CAudioEngine::getInstance()->init());
			int SoundID = CAudioEngine::getInstance()->playAudio2D(constant::FILE_PATH_TEST_AUDIO_01);
			Assert::IsTrue(SoundID >= 0); //HACK: 这里认为只要SoundID >= 0即播放成功，这样做是否合理

			std::this_thread::sleep_for(std::chrono::seconds(1));

			CAudioEngine::getInstance()->destroy();
		}

		TEST_METHOD(playAudio2DWithoutInit)
		{
			int SoundID = CAudioEngine::getInstance()->playAudio2D(constant::FILE_PATH_TEST_AUDIO_01);
			Assert::IsTrue(SoundID >= 0);

			std::this_thread::sleep_for(std::chrono::seconds(1));

			CAudioEngine::getInstance()->destroy();
		}
	};
}