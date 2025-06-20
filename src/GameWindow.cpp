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
#include "Circle.h"
#include "CircleManager.h"

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Camera.h"
#include "CircleManager.h"

#include "Debug.h"
#include "Random.h"
#include "Colors.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);
	InputSystem::Initialize();
	CameraManager::Initialize(this->m_width, this->m_height);

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	std::vector<float> clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderSystem->SetClearColor(clearColor);

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	auto circleMan = std::make_shared<CircleManager>();
	GameObjectManager::GetInstance()->AddGameObject(circleMan, false);

	auto cam = std::make_shared<Camera>("Camera_1", this->m_width, this->m_height);
	cam->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
	cam->SetLookAt(Vector3(0.0f, 0.0f, 0.0f));
	CameraManager::GetInstance()->AddCamera(cam, true);

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
