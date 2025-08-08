#include "pch.h"
#include "SceneReader.h"
#include "GameObjectManager.h"
#include "GameObjectBuilder.h"
#include "MaterialTypes.h"
#include "GameEntity.h"
#include "Debug.h"
#include "PhysicsComponent.h" // [NEW] Include for setting the body type
#include <reactphysics3d/reactphysics3d.h> // [NEW] Include for rp3d::BodyType enum

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

    ParseFile(file);

    Debug::Log("Scene Loaded from: " + fileName);
}

void SceneReader::ParseFile(std::ifstream& file)
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
            std::stringstream ss(line.substr(10));
            ss >> current.position.x >> current.position.y >> current.position.z;
        }
        else if (line.find("Rotation:") == 0) {
            std::stringstream ss(line.substr(10));
            ss >> current.rotation.x >> current.rotation.y >> current.rotation.z >> current.rotation.w;
        }
        else if (line.find("Scale:") == 0) {
            std::stringstream ss(line.substr(7));
            ss >> current.scale.x >> current.scale.y >> current.scale.z;
        }
        else if (line.find("MeshType:") == 0) {
            current.meshType = line.substr(10);
        }
        // [MODIFIED] Look for "RigidbodyType" instead of "HasRigidbody"
        else if (line.find("RigidbodyType:") == 0) {
            current.rigidbodyType = line.substr(15);
        }
        else if (line.empty() && readingObject) {
            if (!current.name.empty()) {
                parsedObjects[current.name] = current;
            }
            readingObject = false;
        }
    }

    if (readingObject && !current.name.empty())
        parsedObjects[current.name] = current;

    std::unordered_map<String, std::shared_ptr<AGameObject>> createdObjects;

    // === Pass 1: Create all objects with transform ===
    for (auto& [name, data] : parsedObjects)
    {
        auto obj = GameObjectBuilder()
            .SetName(name)
            .AddTransformComponent(name)
            .Build();

        obj->Transform()->SetPosition(data.position);
        obj->Transform()->SetScale(data.scale);
        obj->Transform()->SetRotation(data.rotation);
        createdObjects[name] = obj;
    }

    // === Pass 2: Add mesh & physics, and configure physics [MODIFIED] ===
    for (auto& [name, data] : parsedObjects)
    {
        auto obj = std::dynamic_pointer_cast<GameEntity>(createdObjects[name]);
        if (!obj) continue;

        auto builder = GameObjectBuilder().SetExisting(obj);

        if (data.meshType != "None" && data.meshType != "UnknownRenderer")
            builder.AddMeshComponent(data.meshType, MaterialType::DEFAULT);

        // Add physics component if specified in the file
        if (data.rigidbodyType != "None")
        {
            builder.AddPhysicsComponent(data.meshType, false);
        }

        // Build the object with all its components
        auto builtObj = builder.Build();

        // If a physics component was added, find it and set its specific type
        if (data.rigidbodyType != "None")
        {
            if (auto physCompBase = builtObj->FindComponentOfType(AComponent::ComponentType::Physics))
            {
                if (auto physComp = dynamic_cast<PhysicsComponent*>(physCompBase))
                {
                    if (data.rigidbodyType == "Static")
                    {
                        physComp->SetBodyType(reactphysics3d::BodyType::STATIC);
                    }
                    else if (data.rigidbodyType == "Dynamic")
                    {
                        physComp->SetBodyType(reactphysics3d::BodyType::DYNAMIC);
                    }
                    else if (data.rigidbodyType == "Kinematic")
                    {
                        physComp->SetBodyType(reactphysics3d::BodyType::KINEMATIC);
                    }
                }
            }
        }
        createdObjects[name] = builtObj;
    }

    // === Pass 3: Rebuild parent hierarchy and add to manager ===
    for (auto& [name, data] : parsedObjects)
    {
        auto child = createdObjects[name];
        if (!child) continue;

        if (data.parentName != "None")
        {
            if (auto parentIt = createdObjects.find(data.parentName); parentIt != createdObjects.end())
            {
                parentIt->second->AttachChild(child);
            }
        }
        else
        {
            GameObjectManager::GetInstance()->AddGameObject(child);
        }
    }

    Debug::Log("Scene loaded successfully.");
}