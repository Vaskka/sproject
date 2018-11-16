#pragma once

#include "RenderEngineImp.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace sengine
{
	namespace srenderer
	{
		class CGLRenderEngine : public IRenderEngineImp
		{
		public:
			CGLRenderEngine();
			virtual ~CGLRenderEngine();

		protected:
			virtual bool _initV() override;
			virtual bool _renderV() override;

			virtual bool _isRenderLoopDoneV() override;

		private:
			bool __createGLFWWindow();

		private:
			GLFWwindow *m_pWindow;
		};
	}
}