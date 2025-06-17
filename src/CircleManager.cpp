#include "CircleManager.h"
#include "InputSystem.h"
#include "Windows.h"

CircleManager::CircleManager() : AGameObject("Circle Manager")
{
	InputSystem::AddListener();
}

void CircleManager::OnKeyPressed(int key)
{
	this->m_heldKeys.insert(key);
}

void CircleManager::OnKeyReleased(int key)
{
	this->m_heldKeys.erase(key);
}

void CircleManager::SpawnCircle()
{
}

void CircleManager::CloseApp()
{
	::PostQuitMessage(0);
}

void CircleManager::DeleteCircles()
{
}

void CircleManager::Update(float deltaTime)
{
	if (m_heldKeys.contains(VK_SPACE)) SpawnCircle();
	if (m_heldKeys.contains(VK_ESCAPE)) CloseApp();
	if (m_heldKeys.contains(VK_LSHIFT)) DeleteCircles();
}

void CircleManager::Draw(DeviceContext* dvcContext)
{

}
