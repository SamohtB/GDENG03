#include "GameWindow.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"

#include "Triangle.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();

	auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();

	std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(0, "Test Triangle", device);
	GameObjectManager::GetInstance()->AddGameObject(triangle);
}

void GameWindow::OnUpdate()
{
	/* get delta time here and pass to game object manager */

	GameObjectManager::GetInstance()->UpdateAll();
}

void GameWindow::OnRender()
{
	auto cmdList = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandList();

	GraphicsEngine::GetInstance()->GetRenderSystem()->StartFrame();
	
	GameObjectManager::GetInstance()->RenderAll(cmdList);

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	GraphicsEngine::Destroy();
	GameObjectManager::Destroy();
}
