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

	GraphicsEngine::GetInstance()->GetBatchUploader()->StartUpload();

	/* Load Single Cube */
	//auto cube = std::make_shared<Cube>("Cube_1");
	//GameObjectManager::GetInstance()->AddGameObject(cube);

	//Load_4();
	//Load_6();
	Load_7();

	GraphicsEngine::GetInstance()->GetBatchUploader()->StopAndWaitUpload();
}

void GameWindow::Load_6()
{
	auto cube = std::make_shared<Cube>("Cube_1");
	cube->SetPosition(0.0f, 0.9f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_2");
	cube->SetPosition(-1.5f, 2.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_3");
	cube->SetPosition(-1.5f, 3.0f, -2.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	auto plane = std::make_shared<Plane>("Plane_1");
	GameObjectManager::GetInstance()->AddGameObject(plane);
}

void GameWindow::Load_4()
{
	for (int i = 0; i < 50; i++)
	{
		auto cube = std::make_shared<Cube>("Cube_" + std::to_string(i));
		cube->SetPosition(Random::Range(-4.5f, 4.5f), Random::Range(-3.75f, 3.75f), Random::Range(0.0f, 3.0f));
		GameObjectManager::GetInstance()->AddGameObject(cube);
	}
}

void GameWindow::Load_7()
{
	/* Bottom Stacks */
	auto cube = std::make_shared<Cube>("Cube_White_Left_1");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-3.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_1");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-2.5f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Left_2");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-2.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_2");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-1.5f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Left_3");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-1.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_3");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-0.5f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	/* Flat 1 */
	cube = std::make_shared<Cube>("Flat_1");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-90.0f, 90.0f, 0.0f);
	cube->SetPosition(-2.5f, 1.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	/* Flat 2 */
	cube = std::make_shared<Cube>("Flat_2");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-90.0f, 90.0f, 0.0f);
	cube->SetPosition(-1.0f, 1.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	/* Second Layer */
	cube = std::make_shared<Cube>("Cube_White_Left_4");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-2.5f, 2.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_4");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-2.0f, 2.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Left_5");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-1.5f, 2.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_5");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-1.0f, 2.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	/* Flat 3 */
	cube = std::make_shared<Cube>("Flat_3");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-90.0f, 90.0f, 0.0f);
	cube->SetPosition(-1.75f, 3.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	/* Third Layer */
	cube = std::make_shared<Cube>("Cube_White_Left_6");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, -90.0f, 0.0f);
	cube->SetPosition(-2.0f, 4.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);

	cube = std::make_shared<Cube>("Cube_White_Right_6");
	cube->SetScale(1.0f, 2.0f, 0.01f);
	cube->SetRotation(-14.0f, 90.0f, 0.0f);
	cube->SetPosition(-1.5f, 4.0f, 0.0f);
	GameObjectManager::GetInstance()->AddGameObject(cube);
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
