#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"

Inspector::Inspector() : AUIScreen("Inspector")
{
}

void Inspector::DrawUI()
{
    ImGui::Begin("Inspector");

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();

    if (object != nullptr)
    {
        DrawTransformTab(object);
    }

    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

void Inspector::DrawTransformTab(AGameObject* object)
{
    Vector3 position = object->GetLocalPosition();
    Vector3 rotation = object->GetLocalRotation();
    Vector3 scale = object->GetLocalScale();

    ImGui::Text("Object: %s", object->GetName().c_str());

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