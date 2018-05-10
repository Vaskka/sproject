#include "yggRendering.h"
#include "constants.h"

int main()
{
	COpenGLRendering* pRendering = new CYGGRendering();
	_ASSERT(pRendering);

	pRendering->initV(Constant::WIN_NAME, Constant::WIN_WIDTH, Constant::WIN_HEIGHT);
	pRendering->runV();

	delete pRendering;

	return 0;
}