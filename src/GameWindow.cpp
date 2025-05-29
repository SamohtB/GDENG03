#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "TextureManager.h"

#include "RenderSystem.h"
#include "Quad.h"
#include "Triangle.h"


GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();
	TextureManager::Initialize();

	auto render = GraphicsEngine::GetInstance()->GetRenderSystem();

	std::vector<XMFLOAT4> colors =
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.0f, 1.0f}
	};

	std::vector<XMFLOAT4> green =
	{
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
	};

	render->StartResourceUpload();

	std::shared_ptr<Quad> r_rectangle = std::make_shared<Quad>(0, "Rainbow Rectangle", XMFLOAT2(-0.66f, 0.66f), colors);
	GameObjectManager::GetInstance()->AddGameObject(r_rectangle);

	std::shared_ptr<Quad> g_rectangle = std::make_shared<Quad>(1, "Green Rectangle", XMFLOAT2(0.66f, -0.66f), green);
	GameObjectManager::GetInstance()->AddGameObject(g_rectangle);

	std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(2, "Rainbow Triangle", XMFLOAT2(0.0f, 0.0f), colors);
	GameObjectManager::GetInstance()->AddGameObject(triangle);

	render->EndResourceUpload();
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
	TextureManager::Destroy();
	GameObjectManager::Destroy();
	GraphicsEngine::Destroy();
}
