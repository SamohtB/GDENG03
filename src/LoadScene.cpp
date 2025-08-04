#include "pch.h"
#include "LoadScene.h"

LoadScene::LoadScene() : AUIScreen("Load Scene")
{
	this->m_sceneReader = std::make_unique<SceneReader>("Scenes/");
	this->m_visible = false;
}

void LoadScene::DrawUI()
{
    if (ImGui::Begin("Load Scene"), &this->m_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)
    {
        ImGui::Text("Select a scene file:");

        static char sceneNameBuffer[128] = "";
        static String selectedFile = "";

        // --- File browser ---
        ImGui::BeginChild("FileList", ImVec2(300, 200), true);
        for (const auto& entry : std::filesystem::directory_iterator("Scenes/"))
        {
            if (!entry.is_regular_file()) continue;

            auto path = entry.path();
            if (path.extension() == ".level")
            {
                String fileName = path.stem().string(); // remove extension

                if (ImGui::Selectable(fileName.c_str(), fileName == selectedFile))
                {
                    selectedFile = fileName;
                    std::strncpy(sceneNameBuffer, fileName.c_str(), sizeof(sceneNameBuffer));
                }
            }
        }
        ImGui::EndChild();

        // --- Load Button ---
        if (ImGui::Button("Load"))
        {
            String sceneName(sceneNameBuffer);
            if (!sceneName.empty())
            {
                this->m_sceneReader->ReadFromFile(sceneName);
                this->m_visible = false;
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            this->m_visible = false;
        }
    }

    ImGui::End();
}
