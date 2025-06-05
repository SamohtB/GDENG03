#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"

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
	EngineTime::Initialize(60);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	renderSystem->LoadInitialResources();

	std::shared_ptr<PBSQuads> quad = std::make_shared<PBSQuads>(0, "RockBase", XMFLOAT2(-0.5f, 0.33f), ROCK_0);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(1, "RockWithPBS", XMFLOAT2(-0.5f, -0.33f), ROCK_1);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(2, "MetalPlateBase", XMFLOAT2(0.0f, 0.33f), METAL_PLATE_0);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(3, "MetalPlateWithPBS", XMFLOAT2(0.0f, -0.33f), METAL_PLATE_1);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(4, "BricksBase", XMFLOAT2(0.5f, 0.33f), BRICKS_0);
	GameObjectManager::GetInstance()->AddGameObject(quad);

	quad = std::make_shared<PBSQuads>(5, "BricksWithPBS", XMFLOAT2(0.5f, -0.33f), BRICKS_1);
	GameObjectManager::GetInstance()->AddGameObject(quad);


	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
}

void GameWindow::OnUpdate()
{
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
