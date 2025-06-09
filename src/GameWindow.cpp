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

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	renderSystem->LoadInitialResources();

	std::shared_ptr<Cube> cube = std::make_shared<Cube>("Cube");
	GameObjectManager::GetInstance()->AddGameObject(cube);

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
} 

void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime * 1.5f;

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
