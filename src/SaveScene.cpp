#include "pch.h"
#include "SaveScene.h"

SaveScene::SaveScene() : AUIScreen("Save Scene")
{
	this->m_sceneWriter = std::make_unique<SceneWriter>("Scenes/");
    this->m_visible = false;
}

void SaveScene::DrawUI()
{
    if(ImGui::Begin("Save Scene"), &this->m_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)
    {
        ImGui::Text("Enter scene name:");
        ImGui::InputText("##SceneName", m_sceneNameBuffer, IM_ARRAYSIZE(m_sceneNameBuffer));

        if (ImGui::Button("Save"))
        {
            String sceneName(m_sceneNameBuffer);
            this->m_sceneWriter->WriteToFile(sceneName);
            this->m_visible = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
			this->m_visible = false;
        }
    }

    ImGui::End(); 
}
