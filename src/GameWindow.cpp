#include "pch.h"
#include "GameWindow.h"

#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "EngineGUIManager.h"
#include "PhysicsSystem.h"
#include "NameRegistry.h"

#include "RenderSystem.h"
#include "BatchUploader.h"
#include "MaterialManager.h"
#include "TextureManager.h"

#include "ActionHistory.h"
#include "DebugWindow.h"

#include "Debug.h"
#include "Random.h"

GameWindow::GameWindow(UINT width, UINT height) : ABaseWindow(width, height) {}

void GameWindow::OnCreate(HWND hwnd)
{
	Debug::Initialize();
	GraphicsEngine::Initialize(this->m_width, this->m_height, hwnd);
	GameObjectManager::Initialize(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
	EngineTime::Initialize(60);
	InputSystem::Initialize();
	CameraManager::Initialize(this->m_width, this->m_height);
	EngineGUIManager::Initialize(hwnd, this->m_width, this->m_height);
	PhysicsSystem::Initialize();
	NameRegistry::Initialize();
	ActionHistory::Initialize();

	auto debugWindow = (DebugWindow*)(EngineGUIManager::GetInstance()->GetUI(UINames::DEBUG));

	Debug::GetInstance()->AssignWindow(debugWindow);
	debugWindow->ClearLog();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	GraphicsEngine::GetInstance()->LoadInitialResources();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
}


void GameWindow::OnUpdate()
{
	auto deltaTime = EngineTime::GetDeltaTime();
	m_ticks += deltaTime;

	InputSystem::GetInstance()->ProcessInput();

	CameraManager::GetInstance()->Update(deltaTime);
	GameObjectManager::GetInstance()->UpdateAll(deltaTime);
	PhysicsSystem::GetInstance()->UpdateAllComponents(deltaTime);

	FrameConstantsData frameData = {};
	frameData.viewMatrix = CameraManager::GetInstance()->GetActiveCameraViewMatrix();
	frameData.projMatrix = CameraManager::GetInstance()->GetActiveCameraProjMatrix();
	frameData.cameraPosition = CameraManager::GetInstance()->GetActiveCameraPosition();

	GraphicsEngine::GetInstance()->GetRenderSystem()->UpdateFrameConstants(frameData);
}

void GameWindow::OnRender()
{
	auto context = GraphicsEngine::GetInstance()->GetRenderSystem()->GetDeviceContext();

	GraphicsEngine::GetInstance()->GetRenderSystem()->BeginFrame();

	GameObjectManager::GetInstance()->RenderAll(context);

	EngineGUIManager::GetInstance()->DrawAllUI();

	GraphicsEngine::GetInstance()->GetRenderSystem()->EndFrame();
}

void GameWindow::OnDestroy()
{
	NameRegistry::Destroy();
	ActionHistory::Destroy();
	EngineGUIManager::Destroy();
	CameraManager::Destroy();
	InputSystem::Destroy();
	GameObjectManager::Destroy();
	PhysicsSystem::Destroy();
	GraphicsEngine::Destroy();
	Debug::Destroy();
}