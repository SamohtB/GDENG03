#include <stdexcept>
#include "Window.h"

Window::Window(UINT width, UINT height, std::wstring name) : ABaseRenderer(width, height, name)
{
	
}

Window::~Window()
{

}

void Window::OnInit()
{
	LoadPipeline();
	LoadAssets();
}

void Window::OnUpdate()
{
}

void Window::OnRender()
{
	PopulateCommandList();

	/* Execute Command List */

	WaitForPreviousFrame();
}

void Window::OnDestroy()
{
	WaitForPreviousFrame();

	/* Add Close Here */
}

void Window::LoadPipeline()
{
}

void Window::LoadAssets()
{
}

void Window::PopulateCommandList()
{
}

void Window::WaitForPreviousFrame()
{
}
