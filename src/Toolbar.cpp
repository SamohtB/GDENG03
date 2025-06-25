#include "Toolbar.h"
#include "GameObjectSpawner.h"
#include "Win32App.h"
#include "EngineTime.h"

Toolbar::Toolbar() : AUIScreen("TOOLBAR")
{

}

void Toolbar::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        FileMenu();
        GameObjects();
        DisplayFPS();
    }

    ImGui::EndMainMenuBar();
}

void Toolbar::GameObjects()
{
    /* Primitive Spawners */
    if (ImGui::BeginMenu("Primitives"))
    {
        if (ImGui::MenuItem("Cube")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_CUBE); }
        if (ImGui::MenuItem("Plane")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_PLANE); }
        if (ImGui::MenuItem("Sphere")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_SPHERE); }
        ImGui::EndMenu();
    }
}

void Toolbar::FileMenu()
{
    /* File Menus*/
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Clear Scene"))
        {
            GameObjectManager::GetInstance()->ClearAllObjects();
        }

        if (ImGui::MenuItem("Close"))
        {
            Win32App::Quit();
        }

        ImGui::EndMenu();
    }
}

void Toolbar::DisplayFPS()
{
    int fps = EngineTime::GetFPS();
    std::string fpsText = "FPS: " + std::to_string(fps);

    float windowWidth = ImGui::GetWindowWidth();
    float textWidth = ImGui::CalcTextSize(fpsText.c_str()).x;
    ImGui::SameLine(windowWidth - textWidth - 20.0f);

    ImGui::Text("%s", fpsText.c_str());
}
