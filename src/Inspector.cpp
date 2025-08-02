#include "pch.h"
#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "MaterialTypes.h"
#include "MeshTypes.h"
#include "GameObjectBuilder.h"
#include "GameEntity.h"

Inspector::Inspector() : AUIScreen("Inspector") {}

void Inspector::DrawUI()
{
    ImGui::Begin("Inspector", &this->m_visible);

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();
    if (object != nullptr)
    {
        ImGui::Text("Name: %s", object->GetName().c_str());
        ImGui::Spacing();
        
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

            if (auto gameEntity = dynamic_cast<GameEntity*>(object))
            {
                builder.SetExisting(std::shared_ptr<GameEntity>(gameEntity, [](GameEntity*) {}));
            }

            const std::string selected = componentOptions[selectedComponentIndex];

            if (selected == "MeshComponent")
            {
                builder.AddMeshComponent(MeshType::PRIMITIVE_CUBE, "Default");
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

