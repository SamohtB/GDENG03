#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"

#include "RenderSystem.h"
#include "Quad.h"
#include "PBSQuads.h"

#include "ConstantBuffer.h"
#include "MaterialTypes.h"

#include <iostream>

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	renderSystem->LoadInitialResources();

	std::shared_ptr<PBSQuads> quad = std::make_shared<PBSQuads>(0, "RockBase", XMFLOAT2(-0.33f, 0.33f), ROCK);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(1, "RockWithNormalAndRough", XMFLOAT2(-0.33f, -0.33f), ROCK);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(2, "MetalPlateBase", XMFLOAT2(0.33f, 0.33f), METAL_PLATE);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(3, "MetalPlateWithNormalAndRough", XMFLOAT2(0.33f, -0.33f), METAL_PLATE);
	GameObjectManager::GetInstance()->AddGameObject(quad);

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

	GameObjectManager::GetInstance()->RenderAll(context);

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	GameObjectManager::Destroy();
	GraphicsEngine::Destroy();
}
