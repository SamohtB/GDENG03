#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "stdafx.h"

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
    void UpdateAll();
    void RenderAll(ID3D12GraphicsCommandList* cmdList);

    void AddGameObject(GameObjectPtr gameObject);
    void DeleteObject(GameObjectPtr game_object);
    void DeleteObjectByName(String name);
    void ClearAllObjects();

private:
    GameObjectManager();
    ~GameObjectManager() = default;
    GameObjectManager(GameObjectManager const&) {}
    GameObjectManager& operator=(GameObjectManager const&) {}

    static GameObjectManager* sharedInstance;

    List m_objectList;
    Table m_objectTable;
};

