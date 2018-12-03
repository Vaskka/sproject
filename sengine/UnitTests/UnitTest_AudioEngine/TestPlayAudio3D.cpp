#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sengine::audioEngine;

static hiveOO::CProductFactory<CIrrklangAudioPlayer> Creator("IrrklangAudioPlayer");

namespace unitTest4AudioEngine
{
	TEST_CLASS(TestPlayAudio3D)
	{
	public:
		TEST_METHOD(initAudioEngine)
		{
			bool Success = CAudioEngine::getInstance()->init();
			Assert::IsTrue(Success);

			Success = CAudioEngine::getInstance()->init();
			Assert::IsTrue(Success);

			CAudioEngine::getInstance()->destroy();
		}

		TEST_METHOD(playAudio3D)
		{

		}

		TEST_METHOD(playAudio3DWithoutInit)
		{

		}
	};
}