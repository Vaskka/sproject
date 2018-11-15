#pragma once
#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGLRenderEngineExport.h"

namespace sengine
{
	namespace renderEngine
	{
		class CGLRenderer
		{
		public:
			CGLRenderer();
			virtual ~CGLRenderer();

			virtual bool initV(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen = false); //TODO: 配置文件
			virtual int runV();

			double getFrameInterval() const { return m_FrameInterval; }

		protected:
			virtual void _renderV() {}
			virtual void _handleEventsV() {}
			virtual bool _isRenderLoopDoneV();

			void _registerKeyCallback(GLFWkeyfun vFun) { glfwSetKeyCallback(m_pWindow, vFun); }	//TODO: 还可以封装
			void _registerCursorPosCallback(GLFWcursorposfun vFun) { glfwSetCursorPosCallback(m_pWindow, vFun); }
			void _registerMouseButtonCallback(GLFWmousebuttonfun vFun) { glfwSetMouseButtonCallback(m_pWindow, vFun); }
			void _registerScrollCallback(GLFWscrollfun vFun) { glfwSetScrollCallback(m_pWindow, vFun); }

		private:
			bool m_IsInitialized = false;
			bool m_IsRenderLoopDone = false;

			GLFWwindow *m_pWindow = nullptr;
			double m_FrameInterval = 0.0f;

			bool __createGLFWWindow(const std::string& vWindowTitle, int vWindowWidth, int vWindowHeight, bool vIsFullScreen = false);
			void __updateFrameInterval();
		};
	}
}
