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

void GameObjectManager::Initialize(ID3D12Device* device)
{
    try 
    {
        sharedInstance = new GameObjectManager(device);
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

UINT GameObjectManager::ReserveSlot()
{
    if (m_nextRenderedSlot < MAX_OBJECT_COUNT * FRAME_COUNT)
        return m_nextRenderedSlot++;

    Debug::LogError("GameObjectManager::ReserveSlot - No available constant buffer slots left.");
    Debug::Assert(false, "Exceeded material constant buffer capacity!");
    return UINT_MAX;
}

GameObjectManager::GameObjectManager(ID3D12Device* device)
{
    this->m_objectConstantsBuffer = std::make_unique<ObjectConstantsBuffer>(device, MAX_OBJECT_COUNT * FRAME_COUNT);
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

    for (const auto& obj : m_renderedObjectList)
    {
        allObjects.push_back(obj.get());
    }

    return allObjects;
}

int GameObjectManager::ActiveObjects()
{
    int activeCount = 0;

    for (const auto& object : this->m_logicObjectList)
    {
        if (object->IsActive())
        {
            activeCount++;
        }
    }

    for (const auto& obj : m_renderedObjectList)
    {
        if (obj->IsActive())
        {
            activeCount++;
        }
    }
    return activeCount;
}

void GameObjectManager::UpdateAll(float deltaTime)
{
    for (const auto& object : this->m_logicObjectList)
    {
        if (object->IsActive())
        {
            object->Update(deltaTime);
        }
    }

    for (const auto& object : this->m_renderedObjectList)
    {
        if (object->IsActive())
        {
            object->Update(deltaTime);
        }
    }
}

void GameObjectManager::RenderAll(DeviceContext* dvcContext)
{
    for (const auto& object : m_renderedObjectList)
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

    if (hasConstantBuffer)
    {
        std::array<UINT, FRAME_COUNT> cbIndices{};

        for (int i = 0; i < FRAME_COUNT; i++)
        {
            cbIndices[i] = this->ReserveSlot();

            ObjectConstantsData objData = {};
            objData.modelMatrix = gameObject->GetLocalMatrix();
            objData.objectId = cbIndices[i];

            m_objectConstantsBuffer->Update(objData, cbIndices[i]);
        }

        gameObject->SetId(cbIndices[0]);
        m_cbMap[cbIndices[0]] = cbIndices;
        m_renderedObjectList.push_back(gameObject);
    }

    else
    {
        gameObject->SetId(this->m_nextLogicSlot);
        this->m_nextLogicSlot++;
        m_logicObjectList.push_back(gameObject);
    }

    const std::string& name = gameObject->GetName();
    if (m_objectTable.contains(name))
    {
        Debug::LogWarning("GameObjectManager::AddGameObject: Object with name '" + name + "' already exists. Overwriting.");
    }

    m_objectTable[name] = gameObject;
}

void GameObjectManager::DeleteObject(GameObjectPtr game_object)
{
    if (game_object)
    {
        /*To Do: Add Unreserve Slot here */
        m_renderedObjectList.erase(std::remove(m_renderedObjectList.begin(), m_renderedObjectList.end(), game_object), m_renderedObjectList.end());
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
    m_renderedObjectList.clear();
    m_logicObjectList.clear();
    m_objectTable.clear();
}

void GameObjectManager::UpdateConstantBuffer(UINT objId, const ObjectConstantsData& data)
{
    auto frameIndex = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCurrentFrameIndex();
    UINT cbIndex = this->m_cbMap[objId][frameIndex];

    m_objectConstantsBuffer->Update(data, cbIndex);
}

D3D12_GPU_VIRTUAL_ADDRESS GameObjectManager::GetObjectConstantsAddress(UINT objectId, UINT frameIndex)
{
    UINT cbIndex = m_cbMap[objectId][frameIndex];
    return m_objectConstantsBuffer->GetGPUVirtualAddress(cbIndex);
}
