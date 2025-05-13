#include "AppWindow.h"

#include "GraphicsEngine.h"

AppWindow::AppWindow(int width, int height) : Window(width, height)
{
	GraphicsEngine::initialize();
}

AppWindow::~AppWindow()
{
	GraphicsEngine::destroy();
}

void AppWindow::onUpdate()
{
}
