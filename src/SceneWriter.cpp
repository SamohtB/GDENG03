#include "pch.h"
#include "SceneWriter.h"
#include "GameObjectManager.h"
#include "GameEntity.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Debug.h"

SceneWriter::SceneWriter(String directory) : m_directory(directory)
{
}

void SceneWriter::WriteToFile(String fileName)
{
    String fullPath = this->m_directory + fileName + ".level";
    
    if (this->m_directory.find(fullPath) != String::npos)
    {
		Debug::Log(fileName + " already exists. Overwritting file!");
    }

    std::ofstream sceneFile;
    sceneFile.open(fullPath, std::ios::out);

    auto allObjects = GameObjectManager::GetInstance()->GetAllObjects();

    for(const auto& object : allObjects)
    {
        SaveObjectData(object, sceneFile);
    }

    sceneFile.close();
	Debug::Log("Scene Successfully Saved at: " + fullPath);
}

void SceneWriter::SaveObjectData(std::shared_ptr<AGameObject> object, std::ofstream& sceneFile)
{
    if (!object) return;
    if (dynamic_pointer_cast<GameEntity>(object) == nullptr)  return;  // Ensure the object is a GameEntity

    // === Write Object Data ===
    sceneFile << "[GameObject]\n";
    sceneFile << "Name: " << object->GetName() << "\n";

    // Write Parent Name
    if (auto parent = object->GetParent(); parent)
        sceneFile << "ParentName: " << parent->GetName() << "\n";
    else
        sceneFile << "ParentName: None\n";

    // === Transform Data ===
    auto transform = object->Transform();
    if (transform)
    {
        Vector3 pos = transform->GetLocalPosition();
        Vector3 scale = transform->GetLocalScale();
        rp3d::Quaternion rot = transform->GetLocalQuaternion();

        sceneFile << "Position: " << pos.x << " " << pos.y << " " << pos.z << "\n";
        sceneFile << "Rotation: " << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << "\n";
        sceneFile << "Scale: " << scale.x << " " << scale.y << " " << scale.z << "\n";
    }

    // === Mesh Data ===
    if (auto meshCompBase = object->FindComponentOfType(AComponent::ComponentType::Renderer))
    {
        if (auto meshComp = dynamic_cast<MeshComponent*>(meshCompBase))
        {
            sceneFile << "MeshType: " << meshComp->GetMeshType() << "\n";
        }
        else
        {
            sceneFile << "MeshType: UnknownRenderer\n";
        }
    }
    else
    {
        sceneFile << "MeshType: None\n";
    }

    // === Physics Data ===
    bool hasRB = object->FindComponentOfType(AComponent::ComponentType::Physics) != nullptr;
    sceneFile << "HasRigidbody: " << (hasRB ? "true" : "false") << "\n";
    sceneFile << "\n";

    auto children = object->GetChildren();

    for (const auto& child : children)
    {
        SaveObjectData(child, sceneFile);
    }
}
