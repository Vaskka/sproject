#include "Constants.h"
#include "YGGRenderer.h"

int main()
{
	CGLRenderer* pRenderer = new CYGGRenderer();
	_ASSERT(pRenderer);

	pRenderer->initV(Constant::WIN_NAME, Constant::WIN_WIDTH, Constant::WIN_HEIGHT);
	pRenderer->runV();

	delete pRenderer;

	return 0;
}