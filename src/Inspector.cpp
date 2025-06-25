#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"

Inspector::Inspector() : AUIScreen("Inspector")
{
}

void Inspector::DrawUI()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, m_bodyColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, m_headerColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, m_headerColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, m_headerColor);
    ImGui::PushStyleColor(ImGuiCol_Text, m_textColor);

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

    ImGui::PopStyleColor(5);
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