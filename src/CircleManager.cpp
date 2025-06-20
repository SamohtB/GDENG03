#include "CircleManager.h"
#include <Windows.h>

#include "InputSystem.h"
#include "Circle.h"
#include "Colors.h"

#include "GameObjectManager.h"
#include "Debug.h"
#include "Random.h"
#include "Win32App.h"

CircleManager::CircleManager() : AGameObject("CircleManager")
{
	// Register input listener
	InputSystem::GetInstance()->AddListener(this);

	for (int i = 0; i < MAX_CIRCLES; ++i)
	{
		auto circle = std::make_shared<Circle>("Circle_" + std::to_string(i), ColorPalette::White);
		circle->SetActive(false);
		circle->SetScale(0.15f, 0.15f, 0.15f);

		GameObjectManager::GetInstance()->AddGameObject(circle);
		m_circlePool.push_back(circle);
	}
}

void CircleManager::Update(float deltaTime)
{
	
}

void CircleManager::OnKeyReleased(int key)
void CircleManager::OnKeyPressed(int key)
{
	if (key == VK_SPACE) SpawnCircle();
	if (key == VK_BACK) DeleteLastestCircle();
	if (key == VK_DELETE) DeleteAllCircles();
	if (key == VK_ESCAPE) CloseApplication();
}

void CircleManager::SpawnCircle()
{
	for (const auto& circle : m_circlePool)
	{
		if (!circle->IsActive())
		{
			Vector3 direction(Random::Range(-1.0f, 1.0f), Random::Range(-1.0f, 1.0f), 0.0f);
			direction.Normalize();

			circle->SetPosition(0.0f, 0.0f, 0.0f);
			circle->SetDirection(direction);
			circle->SetActive(true);

			m_activeCircles.push(circle);
			return;
		}
	}

	Debug::LogWarning("No more circles! ObjectPool is empty");
}

void CircleManager::DeleteLastestCircle()
{
	if (m_activeCircles.empty())
	{
		Debug::LogWarning("No active circles to delete.");
		return;
	}

	auto last = m_activeCircles.top();
	m_activeCircles.pop();
	last->SetActive(false);
}

void CircleManager::DeleteAllCircles()
{
	for (auto circle : m_circlePool)
	{
		circle->SetActive(false);
	}

	while (!m_activeCircles.empty())
		m_activeCircles.pop();
}

void CircleManager::CloseApplication()
{
	Win32App::Quit();
}

void CircleManager::Draw(DeviceContext* dvcContext)
{

}
