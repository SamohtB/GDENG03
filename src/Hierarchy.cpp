#include "pch.h"
#include "Hierarchy.h"
#include "GameObjectManager.h"
#include "AGameObject.h"

Hierarchy::Hierarchy() : AUIScreen("Hierarchy")
{

}

void Hierarchy::DrawUI()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.9f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

    if (ImGui::Begin("Hierarchy", nullptr, window_flags))
    {

        auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

        for (AGameObject* gameObject : objectList)
        {
            DrawGameObjectNodeRecursive(gameObject);
        }
    }

    ImGui::End();

    ImGui::PopStyleColor();
}

void Hierarchy::DrawGameObjectNode(AGameObject* gameObject)
{
    bool isSelected = GameObjectManager::GetInstance()->GetSelectedObject() == gameObject;

    if (isSelected)
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.6f, 0.3f, 1.0f)); // Green for selected
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.8f, 0.5f, 1.0f)); // Hover effect
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Default color
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Hover effect
    }

    if (ImGui::Selectable(gameObject->GetName().c_str(), isSelected))
    {
        if (isSelected)
        {
            GameObjectManager::GetInstance()->SetSelectedObject(nullptr);
        }
        else
        {
            GameObjectManager::GetInstance()->SetSelectedObject(gameObject);
        }
    }

    if (isSelected)
    {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Delete").x - ImGui::GetStyle().ItemSpacing.x * 2);
        ImGui::SetCursorPosY(cursorPos.y + ImGui::GetStyle().ItemSpacing.y / 2);

        if (ImGui::Button("Delete"))
        {
            GameObjectManager::GetInstance()->DeleteObject(gameObject);
            GameObjectManager::GetInstance()->SetSelectedObject(nullptr);
        }
    }

    ImGui::PopStyleColor(2);
}

void Hierarchy::DrawGameObjectNodeRecursive(AGameObject* gameObject)
{
    bool isSelected = GameObjectManager::GetInstance()->GetSelectedObject() == gameObject;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (gameObject->GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;

    std::string label = gameObject->GetName() + "###" + std::to_string(reinterpret_cast<uintptr_t>(gameObject));

    bool nodeOpen = ImGui::TreeNodeEx(label.c_str(), flags);

    // === Selection handling ===
    if (ImGui::IsItemClicked())
    {
        GameObjectManager::GetInstance()->SetSelectedObject(isSelected ? nullptr : gameObject);
    }

    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20.0f);
    std::string buttonID = "##btn" + std::to_string(reinterpret_cast<uintptr_t>(gameObject));
    if (ImGui::Button(buttonID.c_str(), ImVec2(20, 0)))
    {
        ImGui::OpenPopup(buttonID.c_str());
    }

    // === Context menu for deletion ===
    if (ImGui::BeginPopup(buttonID.c_str()))
    {
        if (ImGui::MenuItem("Delete"))
        {
            auto children = gameObject->GetChildren();

            for (const auto& child : children)
            {
                this->ReparentOrPromote(child, gameObject->GetParent());
            }

            GameObjectManager::GetInstance()->DeleteObject(gameObject);
            GameObjectManager::GetInstance()->SetSelectedObject(nullptr);
            ImGui::EndPopup();
            if (nodeOpen)
                ImGui::TreePop();
            return;
        }

        ImGui::EndPopup();
    }

    // === Draw children ===
    if (nodeOpen)
    {
        for (const auto& child : gameObject->GetChildren())
        {
            DrawGameObjectNodeRecursive(child.get());
        }
        ImGui::TreePop();
    }
}

void Hierarchy::ReparentOrPromote(std::shared_ptr<AGameObject> gameObject, AGameObject* parent)
{
    if (parent != nullptr)
    {
        AGameObject* grandParent = parent->GetParent();

        if (grandParent != nullptr)
        {
            grandParent->AttachChild(gameObject);
        }
        else
        {
            GameObjectManager::GetInstance()->AddGameObject(gameObject);
        }
    }
    else
    {
        GameObjectManager::GetInstance()->AddGameObject(gameObject);
    }
}
