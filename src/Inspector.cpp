#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "AMeshObject.h"
#include "MaterialTypes.h"

Inspector::Inspector() : AUIScreen("Inspector")
{
}

void Inspector::DrawUI()
{
    ImGui::Begin("Inspector");

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();

    if (object != nullptr)
    {
		DrawObjectInfo(object);
        DrawTransformTab(object);
		DrawMaterialTab(object);
    }

    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

void Inspector::DrawObjectInfo(AGameObject* object)
{
    ImGui::Text("Name: %s", object->GetName().c_str());
}

void Inspector::DrawTransformTab(AGameObject* object)
{
    Vector3 position = object->GetLocalPosition();
    Vector3 rotation = object->GetLocalRotation();
    Vector3 scale = object->GetLocalScale();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Transform"))
    {
        if (ImGui::DragFloat3("Position", &position.x, 0.1f))
        {
            object->SetPosition(position);
        }
        if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f))
        {
            object->SetRotation(rotation);
        }
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
        {
            object->SetScale(scale);
        }

        ImGui::TreePop();
    }
}

void Inspector::DrawMaterialTab(AGameObject* object)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Material"))
    {
        static const char* materialOptions[] = 
        {
            MaterialType::DEFAULT,
            MaterialType::ROCK,
            MaterialType::METAL_PLATE,
            MaterialType::BRICKS
        };

		auto meshObject = dynamic_cast<AMeshObject*>(object);
        auto currentMat = meshObject->GetMaterial();
        int currentIndex = 0;

        for (int i = 0; i < IM_ARRAYSIZE(materialOptions); ++i)
        {
            if (currentMat == materialOptions[i])
            {
                currentIndex = i;
                break;
            }
        }

        if (ImGui::Combo("Material", &currentIndex, materialOptions, IM_ARRAYSIZE(materialOptions)))
        {
            meshObject->SetMaterial(materialOptions[currentIndex]);
        }

        ImGui::TreePop();
    }

    
}