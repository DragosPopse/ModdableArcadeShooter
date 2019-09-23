#include <iostream>
#include "Engine.h"


int main(int argc, char** argv)
{
	try
	{
		Engine engine;
		engine.Run();
	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}
	return 0;
}