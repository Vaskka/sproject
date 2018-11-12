#pragma once
#include <string>
#include <vector>
#include <GLM/glm.hpp>

namespace Constant
{
	static const std::string WIN_NAME = "YGGDRASIL";
	static const int WIN_WIDTH = 1920;
	static const int WIN_HEIGHT = 1080;

	static const float CAMERA_NEAR = 0.1f;
	static const float CAMERA_FAR = 1000.0f;

	static const glm::vec3 MODEL_INIT_TRANSLATE = { 0.0f, 0.0f, 0.0f };
	static const glm::vec3 MODEL_INIT_SCALE = { 10.0f, 10.0f, 10.0f };

	static const std::string WHITE_NOISE_TEXTURE_PATH = "res/textures/Noise/grayWhiteNoise01.png";
	static const std::vector<std::string> MAIN_BGM_FILE_PATH = { "res/sounds/01.wav","res/sounds/02.wav","res/sounds/03.wav" }; //HACK: bad design
}