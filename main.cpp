#include "App.h"
#include <stdexcept>
#include <iostream>

int main() 
{
	try
	{
		App app = App(1024, 768);

		while (app.IsRun()) {}
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