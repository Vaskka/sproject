#pragma once

#include <string>
#include <boost/tuple/tuple.hpp>

namespace sengine
{
	namespace srenderer
	{
		namespace CONFIG_FILE_TYPE
		{
			const std::string RENDER_ENGINE_CONFIG = "RENDER_ENGINE_CONFIG";
		}

		namespace CONFIG_FILE_NAME
		{
			const std::string RENDER_ENGINE_CONFIG_FILE_NAME = "RenderEngine.config";
		}

		namespace CONFIG_KEYWORD
		{
			const std::string RENDER_ENGINE_SIGNATURE = "RENDER_ENGINE_SIGNATURE";
			const std::string WIN_POSITION = "WIN_POSITION";
			const std::string WIN_SIZE = "WIN_SIZE";
			const std::string WIN_TITLE = "WIN_TITLE";
			const std::string WIN_IS_FULLSCREEN = "WIN_IS_FULLSCREEN";
		}

		namespace CONFIG_DEFAULT
		{
			const std::string DEFAULT_RENDER_ENGINE_SIGNATURE = "srendererGL";
			const boost::tuple<int, int> DEFAULT_WIN_POSITION = boost::make_tuple(100, 100);
			const boost::tuple<int, int> DEFAULT_WIN_SIZE = boost::make_tuple(1024, 768);
			const std::string DEFAULT_WIN_TITLE = "sengine application";
			const bool DEFAULT_WIN_IS_FULLSCREEN = false;
		}
	}
}

#define _USE_FLOAT_MATRIX

#ifdef _USE_FLOAT_MATRIX
typedef float MATRIX_VALUE_TYPE;
#else
typedef double MATRIX_VALUE_TYPE;
#endif