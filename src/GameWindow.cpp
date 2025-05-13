#include "GameWindow.h"

#include "GraphicsEngine.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
}

void GameWindow::OnUpdate()
{
}

void GameWindow::OnRender()
{
	GraphicsEngine::GetInstance()->RenderFrame();
}

void GameWindow::OnDestroy()
{
	GraphicsEngine::Destroy();
}
