#pragma once
#include "pch.h"
#include "FrameConstants.h"
#include "ConstantBuffer.h"

class DeviceContext;
class AGameObject;

class GameObjectManager
{
public:
    using GameObjectPtr = std::shared_ptr<AGameObject>;
    using String = std::string;
    using List =  std::vector<GameObjectPtr>;
    using Table = std::unordered_map<String, GameObjectPtr>;

    static GameObjectManager* GetInstance();
    static void Initialize();
    static void Destroy();

    AGameObject* FindObjectByName(String name);
    std::vector<AGameObject*> GetAllObjects();
    int ActiveObjects();
    void UpdateAll(float deltaTime);
    void RenderAll(DeviceContext* dvcContext);

    void AddGameObject(GameObjectPtr gameObject, bool hasConstantBuffer = true);
    void DeleteObject(AGameObject* game_object);
    void DeleteObjectByName(String name);
    void ClearAllObjects();

    AGameObject* GetSelectedObject() const;
    void SetSelectedObject(AGameObject* object);
private:
    GameObjectManager();
    ~GameObjectManager() = default;
    GameObjectManager(GameObjectManager const&) {}
    GameObjectManager& operator=(GameObjectManager const&) {}

    static GameObjectManager* sharedInstance;

    List m_objectList;
    Table m_objectTable;

	AGameObject* m_selectedObject = nullptr;

    friend class AGameObject;
};

