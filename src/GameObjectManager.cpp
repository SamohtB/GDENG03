#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "AGameObject.h"
#include "Debug.h"

GameObjectManager* GameObjectManager::sharedInstance = nullptr;

GameObjectManager* GameObjectManager::GetInstance()
{
    return sharedInstance;
}

void GameObjectManager::Initialize()
{
    try 
    {
        sharedInstance = new GameObjectManager();
    }
    catch (...)
    {
        Debug::LogError("Game Object Manager creation failed!");
    }
}

void GameObjectManager::Destroy()
{
    delete sharedInstance;
}

GameObjectManager::GameObjectManager()
{
}

AGameObject* GameObjectManager::FindObjectByName(String name)
{
    auto it = m_objectTable.find(name);

    if (it != m_objectTable.end())
    {
        return it->second.get();
    }

    return nullptr;
}

std::vector<AGameObject*> GameObjectManager::GetAllObjects()
{
    std::vector<AGameObject*> allObjects;

    for (const auto& obj : m_objectList)
    {
        allObjects.push_back(obj.get());
    }

    return allObjects;
}

int GameObjectManager::ActiveObjects()
{
    int activeCount = 0;

    for (const auto& object : this->m_objectList)
    {
        if (object->IsActive())
        {
            activeCount++;
        }
    }

    return activeCount;
}

void GameObjectManager::UpdateAll(float deltaTime)
{
    for (const auto& object : this->m_objectList)
    {
        if (object->IsActive())
        {
            object->Update(deltaTime);
        }
    }
}

void GameObjectManager::RenderAll(DeviceContext* dvcContext)
{
    for (const auto& object : m_objectList)
    {
        if (object->IsActive())
        {
            object->Draw(dvcContext);
        }
    }
}

void GameObjectManager::AddGameObject(GameObjectPtr gameObject, bool hasConstantBuffer)
{
	if (!gameObject) return;

    gameObject->SetId(0);
    m_objectList.push_back(gameObject);
    m_objectTable[gameObject->GetName()] = gameObject;
}

void GameObjectManager::DeleteObject(AGameObject* gameObject)
{
    if (!gameObject) return;

    auto nameIt = m_objectTable.find(gameObject->GetName());
    if (nameIt != m_objectTable.end())
    {
        m_objectTable.erase(nameIt);
    }

    m_objectList.erase(
        std::remove_if(
            m_objectList.begin(),
            m_objectList.end(),
            [gameObject](const GameObjectPtr& ptr)
            {
				return ptr.get() == gameObject;
            }),
        m_objectList.end()
    );
}

void GameObjectManager::DeleteObjectByName(String name)
{
    auto it = m_objectTable.find(name);
    if (it == m_objectTable.end()) return;

    GameObjectPtr gameObject = it->second;

    // Remove from rendered object list
    m_objectList.erase(
        std::remove(m_objectList.begin(), m_objectList.end(), gameObject),
        m_objectList.end()
    );

    // Remove from name table
    m_objectTable.erase(it);
}

void GameObjectManager::ClearAllObjects()
{
    m_objectList.clear();
    m_objectTable.clear();
}

AGameObject* GameObjectManager::GetSelectedObject() const
{
    return this->m_selectedObject;
}

void GameObjectManager::SetSelectedObject(AGameObject* object)
{
	if (object == this->m_selectedObject) return; // No change
	this->m_selectedObject = object;
}
