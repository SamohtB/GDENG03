#include "pch.h"
#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "MaterialTypes.h"
#include "GraphicsEngine.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "PipelineStateManager.h"

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
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

