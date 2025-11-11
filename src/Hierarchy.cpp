#include "pch.h"
#include "Hierarchy.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "EngineGUIManager.h"
#include "HotkeySystem.hpp"
#include "IconsMaterialDesign.h"
#include "Debug.h"

Hierarchy::Hierarchy() : AUIScreen("Hierarchy")
{
    HotkeySystem::getInstance()->addListener(this);
}

Hierarchy::~Hierarchy()
{
    HotkeySystem::getInstance()->removeListener(this);
}

void Hierarchy::DrawUI()
{
    if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_MenuBar))
    {
        static char searchBuffer[128] = "";

        if (ImGui::BeginMenuBar())
        {

            if (ImGui::MenuItem(ICON_MD_ADD ICON_MD_ARROW_DROP_DOWN))
            {
                ImGui::OpenPopup("CreateGameObjectsPopup");
            }

            CreateObjectPopup();

            ImGui::TextUnformatted(ICON_MD_SEARCH);
            ImGui::SameLine();

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::InputTextWithHint("##HierarchySearch", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

            if (strlen(searchBuffer) > 0)
            {
                // For example: filter hierarchy items based on searchBuffer
            }

            ImGui::EndMenuBar();
        }

        auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

        if (ImGui::BeginTable("HierarchyTable", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody))
        {
            ImGui::TableSetupColumn("Visible", ImGuiTableColumnFlags_WidthFixed, 30.0f);
            ImGui::TableSetupColumn("Clickable", ImGuiTableColumnFlags_WidthFixed, 30.0f);
            ImGui::TableSetupColumn("Tree", ImGuiTableColumnFlags_WidthStretch);

            auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

            for (auto gameObject : objectList)
            {
                DrawGameObjectNodeRecursive(gameObject);
            }

            ImGui::EndTable();
        }
    }

    ImGui::End();
}

void Hierarchy::CreateObjectPopup()
{
    if (ImGui::BeginPopup("CreateGameObjectsPopup"))
    {
        if (ImGui::MenuItem("Cut"))
        {
            Debug::Log("Cut Command Selected");
        }

        if (ImGui::MenuItem("Copy"))
        {
            Debug::Log("Copy Command Selected");
        }

        if (ImGui::MenuItem("Paste"))
        {
            Debug::Log("Paste Command Selected");
        }

        if (ImGui::MenuItem("Duplicate"))
        {
            Debug::Log("Duplicate Command Selected");
        }

        if (ImGui::MenuItem("Delete"))
        {
            Debug::Log("Delete Command Selected");
        }

        ImGui::EndPopup();
    }
}

void Hierarchy::OnActionPressed(Hotkey::Action action)
{
 
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
