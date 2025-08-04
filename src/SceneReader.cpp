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

    String line;
    while (std::getline(file, line))
    {
        if (line == "[GameObject]")
        {
            ParseGameObject(file);
        }
    }

    Debug::Log("Scene Loaded from: " + fileName);
}

void SceneReader::ParseGameObject(std::ifstream& file)
{
    String name;
    Vector3 position = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };
    rp3d::Quaternion rotation = { 0, 0, 0, 1 };
    String meshType = "None";
    bool hasRigidbody = false;

    String line;
    while (std::getline(file, line))
    {
        if (line.empty()) break;

        std::istringstream iss(line);
        String label;
        iss >> label;

        if (label == "Name:")
        {
            iss >> name;
        }
        else if (label == "Position:")
        {
            iss >> position.x >> position.y >> position.z;
        }
        else if (label == "Rotation:")
        {
            iss >> rotation.x >> rotation.y >> rotation.z >> rotation.w;
        }
        else if (label == "Scale:")
        {
            iss >> scale.x >> scale.y >> scale.z;
        }
        else if (label == "MeshType:")
        {
            iss >> meshType;
        }
        else if (label == "HasRigidbody:")
        {
            String value;
            iss >> value;
            hasRigidbody = (value == "true");
        }
    }

    // === Use GameObjectBuilder ===
    GameObjectBuilder builder;

    builder.SetName(name).AddTransformComponent(name);

    // === Check Mesh ===
    if (meshType != "None" && meshType != "UnknownRenderer")
    {
        builder.AddMeshComponent(meshType, MaterialType::DEFAULT);
    }

    if (hasRigidbody)
    {
        builder.AddPhysicsComponent(meshType, true);
    }

    auto gameObject = std::dynamic_pointer_cast<GameEntity>(builder.Build());

    // Set transform values after it's built
    if (auto transform = gameObject->Transform())
    {
        transform->SetPosition(position);
        transform->SetRotation(rotation);
        transform->SetScale(scale);
    }

    GameObjectManager::GetInstance()->AddGameObject(gameObject);
}
