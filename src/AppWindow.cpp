#include "AppWindow.h"
#include "GraphicsEngine.h"

AppWindow::AppWindow(UINT width, UINT height) : Window(width, height, L"DirectX Application") {}

void AppWindow::OnCreate()
{
	GraphicsEngine::Initialize(this->m_width, this->m_height);
}

void AppWindow::OnUpdate()
{
}

void AppWindow::OnRender()
{
	/* Draw Calls */

	GraphicsEngine::GetInstance()->RenderFrame();
}

void AppWindow::OnDestroy()
{
	GraphicsEngine::GetInstance()->CloseRenderSystem();
}
