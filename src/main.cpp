#include "Win32App.h"
#include "GameWindow.h"
#include <stdexcept>

int main() 
{
	try
	{
		GameWindow game = GameWindow(1024, 768);
		Win32App app = Win32App(&game, L"DirectX Application");

		while (app.IsRun()) 
		{
			app.Broadcast();
		}
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}