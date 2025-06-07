#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"

#include "RenderSystem.h"
#include "Quad.h"
#include "PBSQuads.h"
#include "AnimatedQuad.h"

#include "MaterialBuffer.h"
#include "MaterialTypes.h"

#include "Debug.h"

#include <cmath>

#define M_PI 3.14159265f

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize();
	EngineTime::Initialize(60);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	renderSystem->LoadInitialResources();

	//std::shared_ptr<PBSQuads> quad = std::make_shared<PBSQuads>(0, "RockBase", XMFLOAT2(-0.5f, 0.33f), ROCK_0);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	//quad = std::make_shared<PBSQuads>(1, "RockWithPBS", XMFLOAT2(-0.5f, -0.33f), ROCK_1);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	//quad = std::make_shared<PBSQuads>(2, "MetalPlateBase", XMFLOAT2(0.0f, 0.33f), METAL_PLATE_0);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	//quad = std::make_shared<PBSQuads>(3, "MetalPlateWithPBS", XMFLOAT2(0.0f, -0.33f), METAL_PLATE_1);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	//quad = std::make_shared<PBSQuads>(4, "BricksBase", XMFLOAT2(0.5f, 0.33f), BRICKS_0);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	//quad = std::make_shared<PBSQuads>(5, "BricksWithPBS", XMFLOAT2(0.5f, -0.33f), BRICKS_1);
	//GameObjectManager::GetInstance()->AddGameObject(quad);

	std::shared_ptr<AnimatedQuad> quad = std::make_shared<AnimatedQuad>(0, "Quad", XMFLOAT2(0.0f, 0.0f));
	GameObjectManager::GetInstance()->AddGameObject(quad);


	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
} 

void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime * 1.5f;

	/* Speed Up/Down */
	//float max = 4.0f;
	//float min = 0.25f;

	//float center = (max + min) / 2.0f;
	//float amplitude = (max - min) / 2.0f;
	//float period = 8.0f * M_PI;
	//float frequency = 2.0f * M_PI / period;

	//float speed = center - amplitude * cos(frequency * m_ticks);
	//float t = 0.5f + 0.5f * cos(speed * m_ticks);;

	//if (m_ticks >= period) m_ticks = 0.0f;

	/* Normal Sine Wave */
	float t = 0.5f + 0.5f * sin(m_ticks);

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateGlobalBuffer(t);

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
