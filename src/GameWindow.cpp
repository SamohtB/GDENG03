#include "GameWindow.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "CameraManager.h"

#include "RenderSystem.h"
#include "BatchUploader.h"

#include "Quad.h"
#include "PBSQuads.h"
#include "AnimatedQuad.h"

#include "Cube.h"
#include "PlaneEntity.h"

#include "Debug.h"
#include "Random.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);
	InputSystem::Initialize();
	CameraManager::Initialize(this->m_width, this->m_height);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	auto plane = std::make_shared<PlaneEntity>("Plane");
	plane->SetRotation(15.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(plane);

	auto cube = std::make_shared<Cube>("Cube");
	GameObjectManager::GetInstance()->AddGameObject(cube);

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
} 

void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime;

	InputSystem::GetInstance()->ProcessInput();

	CameraManager::GetInstance()->Update(deltaTime);
	GameObjectManager::GetInstance()->UpdateAll(deltaTime);

	FrameConstantsData frameData = {};
	frameData.viewMatrix = CameraManager::GetInstance()->GetActiveCameraViewMatrix();
	frameData.projMatrix = CameraManager::GetInstance()->GetActiveCameraProjMatrix();;

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateFrameConstants(frameData);
}

void GameWindow::OnRender()
{
	auto context = GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext();

	GraphicsEngine::GetInstance()->GetRenderSystem()->BeginFrame();

	GameObjectManager::GetInstance()->RenderAll(context);

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	CameraManager::Destroy();
	InputSystem::Destroy();
	GameObjectManager::Destroy();
	GraphicsEngine::Destroy();
}
