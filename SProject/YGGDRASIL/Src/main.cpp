#include <iostream>
#include "RenderEngineInterface.h"

int main(int, char**)
{
	try
	{
		sengine::renderEngine::init();
		sengine::renderEngine::run();
	}
	catch (...)
	{
		std::cerr << "The program is terminated due to unexpected error." << std::endl;
	}

	system("pause");

	return 0;
}