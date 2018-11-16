#pragma once
#include <common/BaseProduct.h>
#include "RenderEngineExport.h"

namespace sengine
{
	namespace srenderer
	{
		class RENDER_ENGINE_DLL_EXPORT IRenderEngineImp : public hiveOO::CBaseProduct
		{
			friend class CRenderEngine;

		public:
			virtual ~IRenderEngineImp() {}

		protected:
			IRenderEngineImp() {}

			virtual bool _initV() = 0;
			virtual bool _renderV() = 0;

			virtual bool _isRenderLoopDoneV() { return false; };
		};
	}
}