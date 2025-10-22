#include "pch.h"
#include "Hierarchy.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "EngineGUIManager.h"

Hierarchy::Hierarchy() : AUIScreen("Hierarchy")
{

}

void Hierarchy::DrawUI()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

    if (ImGui::Begin("Hierarchy", nullptr, window_flags))
    {
        EngineGUIManager::BeginToolbarRegion("Heirarchy Toolbar", 28.0f);

        if (ImGui::Button("+", ImVec2(28.0f, 0.0f)))
        {
        }

        ImGui::SameLine();

        {
            const float searchWidth = 150.0f;
            // remaining width after the button
            float avail = ImGui::GetContentRegionAvail().x;
            // current cursor X inside the window (after the button)
            float curX = ImGui::GetCursorPosX();
            // compute target X so the input's right edge aligns with the right content edge
            float targetX = curX + avail - searchWidth;
            // clamp so we don't move backwards past current X
            if (targetX < curX) targetX = curX;
            ImGui::SetCursorPosX(targetX);

            ImGui::SetNextItemWidth(searchWidth);
            static char searchBuffer[128] = "";
            ImGui::InputText("##Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
        }

        EngineGUIManager::EndToolbarRegion();

        auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

        if (ImGui::BeginTable("HierarchyTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody))
        {
            ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed, 30.0f);
            ImGui::TableSetupColumn("Clickable", ImGuiTableColumnFlags_WidthFixed, 30.0f);
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);

            auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

            for (auto gameObject : objectList)
            {
                DrawGameObjectNodeRecursive(gameObject);
            }

            ImGui::EndTable();
        }


        //// === Create an invisible drop target for root ===
        //ImVec2 availableSpace = ImGui::GetContentRegionAvail();
        //if (availableSpace.y > 0)
        //{
        //    ImGui::Dummy(ImVec2(availableSpace.x, availableSpace.y));
        //    if (ImGui::BeginDragDropTarget())
        //    {
        //        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT"))
        //        {
        //            auto draggedShared = *(std::shared_ptr<AGameObject>*)payload->Data;
        //            std::shared_ptr<AGameObject> draggedObject = draggedShared;

        //            if (draggedObject->GetParent() != nullptr)
        //            {
        //                draggedObject->DetachFromParent();
        //                GameObjectManager::GetInstance()->AddGameObject(draggedObject);
        //            }
        //        }
        //        ImGui::EndDragDropTarget();
        //    }
        //}
    }

    ImGui::End();
}

void Hierarchy::DrawGameObjectNode(std::shared_ptr<AGameObject> gameObject)
{
    bool isSelected = GameObjectManager::GetInstance()->GetSelectedObject() == gameObject;

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
    ImGui::TableNextRow();

    // --- Visibility toggle ---
    ImGui::TableSetColumnIndex(0);
    bool visible = true;
    if (ImGui::Checkbox(("##vis" + gameObject->GetName()).c_str(), &visible))
    {
    }


    // --- Clickable toggle ---
    ImGui::TableSetColumnIndex(1);
    bool clickable = true;
    if (ImGui::Checkbox(("##click" + gameObject->GetName()).c_str(), &clickable))
    {

    }

    // --- Name / Tree node ---
    ImGui::TableSetColumnIndex(2);
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
    bool nodeOpen = ImGui::TreeNodeEx((void*)gameObject.get(), nodeFlags, "%s", gameObject->GetName().c_str());

    if (ImGui::IsItemClicked())
        GameObjectManager::GetInstance()->SetSelectedObject(gameObject);

    if (nodeOpen)
    {
        for (auto child : gameObject->GetChildren())
            DrawGameObjectNodeRecursive(child);
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
