#pragma once

#include <string>
#include <GLM/glm.hpp>

namespace Constant
{
	static const std::string WIN_NAME = "YGGDRASIL";
	static const int WIN_WIDTH = 1920;
	static const int WIN_HEIGHT = 1080;

	static const std::string EVN_MAP_PATH = "res/textures/UnderTheSea4k.hdr";

	static const unsigned int EVN_MAP_SIZE = 1024;
	static const unsigned int IRRADIANCE_MAP_SIZE = 128;
	static const unsigned int PREFILTERED_MAP_SIZE = 1024;
	static const unsigned int BRDF_INT_MAP_SIZE = 1024;
	static const unsigned int PREFILTER_MAX_MIP_LEVELS = 6;

	static const float CAMERA_NEAR = 0.1f;
	static const float CAMERA_FAR = 1000.0f;

	static const glm::vec3 MODEL_INIT_TRANSLATE = { 0.0f, 0.0f, 0.0f };
	static const glm::vec3 MODEL_INIT_SCALE = { 10.0f, 10.0f, 10.0f };
}