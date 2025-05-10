#include "AppWindow.h"
#include <stdexcept>
#include <iostream>

int main() 
{
	try
	{
		AppWindow app = AppWindow(1024, 768);
		while (app.IsRun()) {}
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}