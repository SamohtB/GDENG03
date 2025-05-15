#include "GameObjectManager.h"
#include "AGameObject.h"
#include <d3d12.h>

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
        std::exception("Game Object Manager not created sucessfully");
    }
}

void GameObjectManager::Destroy()
{
    delete sharedInstance;
}

GameObjectManager::GameObjectManager() {}

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
    for (const auto& obj : m_objectList)
    {
        if (obj->IsActive())
        {
            activeCount++;
        }
    }
    return activeCount;
}

void GameObjectManager::UpdateAll()
{
    for (const auto& object : this->m_objectList)
    {
        if (object->IsActive())
        {
            object->Update();
        }
    }
}

void GameObjectManager::RenderAll(ID3D12GraphicsCommandList* cmdList)
{
    for (const auto& object : m_objectList)
    {
        if (object->IsActive())
        {
            object->Draw(cmdList);
        }
    }
}

void GameObjectManager::AddGameObject(GameObjectPtr gameObject)
{
    if (gameObject)
    {
        m_objectList.push_back(gameObject);
        m_objectTable[gameObject->GetName()] = gameObject;
    }
}

void GameObjectManager::DeleteObject(GameObjectPtr game_object)
{
    if (game_object)
    {
        m_objectList.erase(std::remove(m_objectList.begin(), m_objectList.end(), game_object), m_objectList.end());
        m_objectTable.erase(game_object->GetName());
    }
}

void GameObjectManager::DeleteObjectByName(String name)
{
    auto it = m_objectTable.find(name);
    if (it != m_objectTable.end())
    {
        DeleteObject(it->second);
    }
}

void GameObjectManager::ClearAllObjects()
{
    m_objectList.clear();
    m_objectTable.clear();
}
