#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "TextureManager.h"

#include "RenderSystem.h"
#include "AnimatedQuad.h"
#include "Quad.h"
#include "Triangle.h"


GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();
	TextureManager::Initialize();

	auto render = GraphicsEngine::GetInstance()->GetRenderSystem();

	render->StartResourceUpload();

	//std::shared_ptr<Quad> r_rectangle = std::make_shared<Quad>(0, "Rainbow Rectangle", XMFLOAT2(-0.66f, 0.66f));
	//GameObjectManager::GetInstance()->AddGameObject(r_rectangle);

	//std::shared_ptr<Quad> g_rectangle = std::make_shared<Quad>(1, "Green Rectangle", XMFLOAT2(0.66f, -0.66f));
	//GameObjectManager::GetInstance()->AddGameObject(g_rectangle);

	//std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(2, "Rainbow Triangle", XMFLOAT2(0.0f, 0.0f));
	//GameObjectManager::GetInstance()->AddGameObject(triangle);

	std::shared_ptr<AnimatedQuad> aquad = std::make_shared<AnimatedQuad>(0, "AQuad");
	GameObjectManager::GetInstance()->AddGameObject(aquad);

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
