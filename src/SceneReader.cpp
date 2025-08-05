#include "pch.h"
#include "SceneReader.h"
#include "GameObjectManager.h"
#include "GameObjectBuilder.h"
#include "MaterialTypes.h"
#include "GameEntity.h"
#include "Debug.h"

SceneReader::SceneReader(String directory) : m_directory(directory)
{
}

void SceneReader::ReadFromFile(const String& fileName)
{
    String fullPath = m_directory + fileName + ".level";

    std::ifstream file(fullPath);

    if (!file.is_open())
    {
        Debug::Log("Failed to open scene file: " + fullPath);
        return;
    }

    ParseGameObject(file);

    Debug::Log("Scene Loaded from: " + fileName);
}

void SceneReader::ParseGameObject(std::ifstream& file)
{
    if (!file.is_open()) return;

    std::unordered_map<String, ObjectData> parsedObjects;
    String line;
    ObjectData current;
    bool readingObject = false;

    while (std::getline(file, line))
    {
        if (line == "[GameObject]") {
            current = ObjectData();
            readingObject = true;
        }
        else if (line.find("Name:") == 0) {
            current.name = line.substr(6);
        }
        else if (line.find("ParentName:") == 0) {
            current.parentName = line.substr(12);
        }
        else if (line.find("Position:") == 0) {
            std::stringstream ss(line.substr(9));
            ss >> current.position.x >> current.position.y >> current.position.z;
        }
        else if (line.find("Rotation:") == 0) {
            std::stringstream ss(line.substr(9));
            ss >> current.rotation.x >> current.rotation.y >> current.rotation.z >> current.rotation.w;
        }
        else if (line.find("Scale:") == 0) {
            std::stringstream ss(line.substr(7));
            ss >> current.scale.x >> current.scale.y >> current.scale.z;
        }
        else if (line.find("MeshType:") == 0) {
            current.meshType = line.substr(10);
        }
        else if (line.find("HasRigidbody:") == 0) {
            current.hasRigidbody = (line.substr(14) == "true");
        }
        else if (line.empty() && readingObject) {
            parsedObjects[current.name] = current;
            readingObject = false;
        }
    }

    // Handle case where last object has no trailing newline
    if (readingObject && !current.name.empty())
        parsedObjects[current.name] = current;

    // === Pass 1: Create all objects with transform ===
    std::unordered_map<String, std::shared_ptr<AGameObject>> createdObjects;

    for (auto& [name, data] : parsedObjects)
    {
        auto builder = GameObjectBuilder()
            .SetName(name)
            .AddTransformComponent(name);

        auto obj = builder.Build();
        obj->Transform()->SetPosition(data.position);
        obj->Transform()->SetScale(data.scale);
        obj->Transform()->SetRotation(data.rotation);

        createdObjects[name] = obj;
    }

    // === Pass 2: Add mesh & physics ===
    for (auto& [name, data] : parsedObjects)
    {
        auto obj = std::dynamic_pointer_cast<GameEntity>(createdObjects[name]);
        if (!obj) continue;

        auto builder = GameObjectBuilder().SetExisting(obj);

        if (data.meshType != "None" && data.meshType != "UnknownRenderer")
            builder.AddMeshComponent(data.meshType, MaterialType::DEFAULT);

        if (data.hasRigidbody)
            builder.AddPhysicsComponent(data.meshType, false);

        createdObjects[name] = builder.Build();
    }

    // === Pass 3: Rebuild parent hierarchy and add to manager ===
    for (auto& [name, data] : parsedObjects)
    {
        auto child = createdObjects[name];
        if (!child) continue;

        if (data.parentName != "None")
        {
            auto parentIt = createdObjects.find(data.parentName);
            if (parentIt != createdObjects.end())
            {
                auto parent = parentIt->second;
                parent->AttachChild(child);
            }
                
        }
        else if (data.parentName == "None")
        {
            GameObjectManager::GetInstance()->AddGameObject(child);
        }
    }

    Debug::Log("Scene loaded successfully.");
}
