#include "pch.h"
#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "MaterialTypes.h"
#include "MeshTypes.h"
#include "GameObjectBuilder.h"
#include "GameEntity.h"

#include "IconsMaterialDesign.h"

Inspector::Inspector() : AUIScreen("Inspector") {}

void Inspector::DrawUI()
{
    ImGui::Begin(("Inspector " + static_cast<String>(ICON_MD_ADD_ROAD)).c_str(), &this->m_visible);	

    std::shared_ptr<AGameObject> object = GameObjectManager::GetInstance()->GetSelectedObject();

    if (object != nullptr)
    {
		ImGui::BeginTable("GameObject_Properties", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody);
        {
            float toggleWidth = 24.0f;
            float labelWidth = ImGui::CalcTextSize("Static").x;
			ImGui::TableSetupColumn("Active", ImGuiTableColumnFlags_WidthFixed, toggleWidth);
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Static", ImGuiTableColumnFlags_WidthFixed, labelWidth + toggleWidth);

			ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            bool tempActive = GameObjectManager::GetInstance()->GetSelectedObject()->IsActive();
            ImGui::Checkbox("##Active", &tempActive);

            ImGui::TableNextColumn();
            ImGui::TableSetColumnIndex(1);

            char nameBuf[256];
            std::strncpy(nameBuf, GameObjectManager::GetInstance()->GetSelectedObject()->GetName().c_str(), sizeof(nameBuf));
            nameBuf[sizeof(nameBuf) - 1] = '\0';
            String tempName = GameObjectManager::GetInstance()->GetSelectedObject()->GetName();

            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
            bool nameEdited = ImGui::InputText("##Name", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue);

            if (nameEdited || ImGui::IsItemDeactivatedAfterEdit())
            {
                GameObjectManager::GetInstance()->GetSelectedObject()->SetName(String(nameBuf));
            }

            ImGui::TableNextColumn();
            ImGui::TableSetColumnIndex(2);

            bool tempStatic = true;
            ImGui::Checkbox("Static", &tempStatic);

            ImGui::EndTable();
        }

		ImGui::AlignTextToFramePadding();
        ImGui::Text("Tag");
		ImGui::PushItemWidth(175.0f);
        ImGui::SameLine();
        static int selectedTagIndex = 0;
        ImGui::Combo("##Tag", &selectedTagIndex, "Default\0UI\0Background\0Foreground\0");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Layer");
        ImGui::PushItemWidth(175.0f);
        ImGui::SameLine();
        static int selectedLayerIndex = 0;
        ImGui::Combo("##Layer", &selectedLayerIndex, "Default\0UI\0Background\0Foreground\0");
        ImGui::PopItemWidth();

        ImGui::Separator();
        
        auto components = object->GetAllComponents();

        for (const auto& component : components)
        {
            component->DrawUI();
        }

        ImGui::SeparatorText("Add Component");

        static const char* componentOptions[] = { "MeshComponent", "PhysicsComponent" };
        static int selectedComponentIndex = 0;

        ImGui::Combo("Component Type", &selectedComponentIndex, componentOptions, IM_ARRAYSIZE(componentOptions));

        if (ImGui::Button("Add"))
        {
            GameObjectBuilder builder;

            if (auto gameEntity = dynamic_pointer_cast<GameEntity>(object))
            {
                builder.SetExisting(gameEntity);
            }

            const std::string selected = componentOptions[selectedComponentIndex];

            if (selected == "MeshComponent")
            {
                builder.AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT);
            }
            else if (selected == "PhysicsComponent")
            {
                builder.AddPhysicsComponent(MeshType::PRIMITIVE_CUBE, true);
            }
        }
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

