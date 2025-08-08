#include "pch.h"
#include "SceneWriter.h"
#include "GameObjectManager.h"
#include "GameEntity.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "Debug.h"
#include "PhysicsComponent.h"

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

    // === Physics Data (MODIFIED) ===
    if (auto physicsCompBase = object->FindComponentOfType(AComponent::ComponentType::Physics))
    {
        if (auto physicsComp = dynamic_cast<PhysicsComponent*>(physicsCompBase))
        {
            // Use the new GetBodyType() method
            reactphysics3d::BodyType bodyType = physicsComp->GetBodyType();
            switch (bodyType)
            {
            case reactphysics3d::BodyType::STATIC:
                sceneFile << "RigidbodyType: Static\n";
                break;
            case reactphysics3d::BodyType::KINEMATIC:
                sceneFile << "RigidbodyType: Kinematic\n";
                break;
            case reactphysics3d::BodyType::DYNAMIC:
                sceneFile << "RigidbodyType: Dynamic\n";
                break;
            }
        }
    }
    else
    {
        sceneFile << "RigidbodyType: None\n";
    }
    sceneFile << "\n";

    // Recursively save children
    auto children = object->GetChildren();
    for (const auto& child : children)
    {
        SaveObjectData(child, sceneFile);
    }
}

