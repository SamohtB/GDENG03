#include "AppWindow.h"
#include <stdexcept>
#include <iostream>

int main() 
{
	try
	{
		AppWindow app;

		while (app.isRun()) 
		{
			app.broadcast();
		}
	}
	catch (const std::runtime_error&)
	{
		return EXIT_FAILURE;
	}
	catch (const std::exception&)
	{
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "App Run Error" << std::endl;
	}

	return EXIT_SUCCESS;
}