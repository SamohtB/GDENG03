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

        for (auto gameObject : objectList)
        {
            DrawGameObjectNodeRecursive(gameObject);
        }

        // === Create an invisible drop target for root ===
        ImVec2 availableSpace = ImGui::GetContentRegionAvail();
        if (availableSpace.y > 0)
        {
            ImGui::Dummy(ImVec2(availableSpace.x, availableSpace.y));
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
                {
                    auto draggedShared = *(std::shared_ptr<AGameObject>*)payload->Data;
                    std::shared_ptr<AGameObject> draggedObject = draggedShared;

                    if (draggedObject->GetParent() != nullptr)
                    {
                        draggedObject->DetachFromParent();
                        GameObjectManager::GetInstance()->AddGameObject(draggedObject);
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
    }

    ImGui::End();

    ImGui::PopStyleColor();
}

void Hierarchy::DrawGameObjectNode(std::shared_ptr<AGameObject> gameObject)
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

void Hierarchy::DrawGameObjectNodeRecursive(std::shared_ptr<AGameObject> gameObject)
{
    bool isSelected = GameObjectManager::GetInstance()->GetSelectedObject() == gameObject;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (gameObject->GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool nodeOpen = ImGui::TreeNodeEx(gameObject->GetName().c_str(), flags);

    // === Handle Selection ===
    if (ImGui::IsItemClicked())
    {
        GameObjectManager::GetInstance()->SetSelectedObject(isSelected ? nullptr : gameObject);
    }

    // === Drag Source ===
    if (ImGui::BeginDragDropSource())
    {
        std::shared_ptr<AGameObject> sharedGameObject = gameObject;
        ImGui::SetDragDropPayload("GAMEOBJECT", &sharedGameObject, sizeof(std::shared_ptr<AGameObject>));
        ImGui::Text("Move %s", gameObject->GetName().c_str());
        ImGui::EndDragDropSource();
    }

    // === Drop Target ===
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
        {
            auto draggedShared = *(std::shared_ptr<AGameObject>*)payload->Data;
            std::shared_ptr<AGameObject> draggedObject = draggedShared;

            if (draggedObject != gameObject && !draggedObject->IsDescendantOf(gameObject))
            {
                draggedObject->DetachFromParent();
                gameObject->AttachChild(draggedShared);
                GameObjectManager::GetInstance()->DeleteObject(draggedShared);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // === Right-click context menu (Delete) ===
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 20.0f);
    std::string buttonID = "##btn" + std::to_string(reinterpret_cast<uintptr_t>(gameObject.get()));
    if (ImGui::Button(buttonID.c_str(), ImVec2(20, 0)))
    {
        ImGui::OpenPopup(buttonID.c_str());
    }

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

    // === Children Recursion ===
    if (nodeOpen)
    {
        for (const auto& child : gameObject->GetChildren())
        {
            DrawGameObjectNodeRecursive(child);
        }
        ImGui::TreePop();
    }
}


void Hierarchy::ReparentOrPromote(std::shared_ptr<AGameObject> gameObject, std::shared_ptr<AGameObject> parent)
{
    if (parent != nullptr)
    {
        auto grandParent = parent->GetParent();

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
