#include "yggRendering.h"

int main()
{
	auto pYGGRendering = new CYGGRendering();

	_ASSERT(pYGGRendering);

	pYGGRendering->init();
	pYGGRendering->draw();

	delete pYGGRendering;

	return 0;
}