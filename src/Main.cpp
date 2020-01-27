#include <iostream>

#include "Engine.h"


int main()
{
	try
	{
		Engine engine;
		engine.Run();
	}
	catch (std::exception& e) //catch any unhandled exception and display the message
	{
		std::cout << e.what();
	}
	return 0;
}