#include "GameWindow.h"

#include "stdafx.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"

#include "RenderSystem.h"
#include "Quad.h"
#include "PBSQuads.h"
#include "AnimatedQuad.h"
#include "Cube.h"

#include "Debug.h"
#include "Random.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	renderSystem->LoadInitialResources();

	float depth = 3.0f;
	float zPos = depth;

	float fovY = DirectX::XMConvertToRadians(60.0f);
	float tanFovY = tanf(fovY * 0.5f);
	float viewHeight = 2.0f * depth * tanFovY;
	float viewWidth = viewHeight * this->m_aspectRatio;

	float padding = 0.8f;

	// Spawn 100 cubes
	for (int i = 0; i < 100; ++i)
	{
		std::shared_ptr<Cube> cube = std::make_shared<Cube>("Cube_" + std::to_string(i));

		float x = Random::Range(-viewWidth * 0.5f * padding, viewWidth * 0.5f * padding);
		float y = Random::Range(-viewHeight * 0.5f * padding, viewHeight * 0.5f * padding);
		float z = 0.0f;

		cube->SetPosition(Vector3(x, y, -5.0f + zPos + z));
		cube->SetScale(0.5f, 0.5f, 0.5f);
		cube->SetRandomRotation();

		GameObjectManager::GetInstance()->AddGameObject(cube);
	}

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
} 

void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime;

	Vector3 cameraPosition = Vector3(0.0f, 0.0f, -5.0f); 
	Vector3 cameraTarget = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 upDirection = Vector3(0.0f, 1.0f, 0.0f);
	constexpr float fovRadians = DirectX::XMConvertToRadians(60.0f);

	FrameConstantsData frameData = {};
	frameData.viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(cameraPosition, cameraTarget, upDirection);
	frameData.projMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadians, this->m_aspectRatio, 0.1f, 100.0f);

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateFrameConstants(frameData);

	/* get delta time here and pass to game object manager */
	GameObjectManager::GetInstance()->UpdateAll(deltaTime);
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
