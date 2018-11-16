#pragma once
#include <common/Singleton.h>
#include "RenderEngineConfig.h"

namespace sengine
{
	namespace srenderer
	{
		class IRenderEngineImp;
		class CDisplayDevice;
		class CScene;
		class CRenderEngineConfig;

		class CRenderEngine : public hiveOO::CSingleton<CRenderEngine>
		{
			friend class hiveOO::CSingleton<CRenderEngine>;

		public:
			virtual ~CRenderEngine();

			bool init();
			bool run();

			IRenderEngineImp*	fetchRenderEngine() const { return m_pRenderEngineImp; }
			CDisplayDevice*		fetchDisplayDevice() const { return m_pDisplayDevice; }

		private:
			CRenderEngine();

			bool __render();
			bool __loadRenderEngineImpLibrary(const std::string& vEngineSig);
			void __createWindow();

		private:
			bool m_IsInitialized;
			bool m_IsRunning;
			bool m_IsRenderLoopDone;

			IRenderEngineImp	*m_pRenderEngineImp;
			CDisplayDevice		*m_pDisplayDevice;
			CScene				*m_pScene;

			CRenderEngineConfig m_RenderEngineConfig;
		};
	}
}