#include "Constants.h"
#include "YGGRenderer.h"

int main()
{
	CYGGRenderer Renderer;
	Renderer.initV(Constant::WIN_NAME, Constant::WIN_WIDTH, Constant::WIN_HEIGHT, true);

	return Renderer.runV();
}