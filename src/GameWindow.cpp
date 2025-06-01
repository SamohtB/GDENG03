#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"

#include "RenderSystem.h"
#include "Quad.h"

#include "ConstantBuffer.h"

#include <iostream>

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	GraphicsEngine::GetInstance()->GetTextureManager()->LoadInitialTextures();

	std::shared_ptr<Quad> rectangle = std::make_shared<Quad>(0, "Rectangle_1", XMFLOAT2(0.0f, 0.66f));
	GameObjectManager::GetInstance()->AddGameObject(rectangle);

	rectangle = std::make_shared<Quad>(1, "Rectangle_2", XMFLOAT2(0.0f, 0.0f));
	GameObjectManager::GetInstance()->AddGameObject(rectangle);

	rectangle = std::make_shared<Quad>(2, "Rectangle_3", XMFLOAT2(0.0f, -0.66f));
	GameObjectManager::GetInstance()->AddGameObject(rectangle);

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
}

void GameWindow::OnUpdate()
{
	unsigned long newTime = 0;

	if (m_oldTime)
		newTime = ::GetTickCount64() - m_oldTime;

	m_deltaTime = newTime / 1000.0f;

	m_oldTime = ::GetTickCount64();
	m_angle += 1.57f * m_deltaTime;

	/* get delta time here and pass to game object manager */
	GameObjectManager::GetInstance()->UpdateAll();
}

void GameWindow::OnRender()
{
	auto context = GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext();

	GraphicsEngine::GetInstance()->GetRenderSystem()->StartFrame();

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateConstantBuffer(m_angle); // Update Constant Buffers when GPU not busy

	GameObjectManager::GetInstance()->RenderAll(context);

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	GameObjectManager::Destroy();
	GraphicsEngine::Destroy();
}
