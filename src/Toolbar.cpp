#include "pch.h"

#include "Toolbar.h"
#include "EngineGUIManager.h"

#include "Win32App.h"
#include "EngineTime.h"

#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include "GameObjectManager.h"
#include "GameObjectSpawner.h"
#include "GameObjectBuilder.h"
#include "TransformComponent.h"

#include "NameRegistry.h"
#include "ActionHistory.h"

Toolbar::Toolbar() : AUIScreen("TOOLBAR")
{
	
}

void Toolbar::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        FileMenu();
        GameObjects();
        Lighting();
        Windows();
        UndoRedo();
        DisplayFPS();
    }

    ImGui::EndMainMenuBar();
}

void Toolbar::Lighting()
{
    if (ImGui::BeginMenu("Lighting"))
    {
        if (ImGui::MenuItem("Point Light")) { GameObjectSpawner::CreateLight(); }
        ImGui::EndMenu();
    }
}

void Toolbar::GameObjects()
{
    if (ImGui::BeginMenu("Game Objects"))
    {
        // -------------------- Primitives --------------------
        if (ImGui::BeginMenu("Primitives"))
        {
            if (ImGui::MenuItem("Cube"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Cube");
                auto cube = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                    .Build();

                if (cube)
                {
                    GameObjectManager::GetInstance()->AddGameObject(cube);
                }
                else
                {
                    Debug::LogError("Cube not created! Error in primitive spawn menu");
                }
            }

            if (ImGui::MenuItem("Plane"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Plane");
                auto plane = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_PLANE, MaterialType::DEFAULT)
                    .Build();

                if (plane)
                {
                    GameObjectManager::GetInstance()->AddGameObject(plane);
                }
                else
                {
                    Debug::LogError("Plane not created! Error in primitive spawn menu");
                }
            }

            if (ImGui::MenuItem("Sphere"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Sphere");
                auto sphere = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_SPHERE, MaterialType::DEFAULT)
                    .Build();

                if (sphere)
                {
                    GameObjectManager::GetInstance()->AddGameObject(sphere);
                }
                else
                {
                    Debug::LogError("Sphere not created! Error in primitive spawn menu");
                }
            }

            if (ImGui::MenuItem("Cylinder"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Cylinder");
                auto cylinder = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_CYLINDER, MaterialType::DEFAULT)
                    .Build();

                if (cylinder)
                {
                    GameObjectManager::GetInstance()->AddGameObject(cylinder);
                }
                else
                {
                    Debug::LogError("Cylinder not created! Error in primitive spawn menu");
                }
            }
            ImGui::EndMenu();
        }

        // -------------------- Physics Objects --------------------
        if (ImGui::BeginMenu("Physics Objects"))
        {
            if (ImGui::MenuItem("Dynamic Physics Cube"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsCube");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                    .AddPhysicsComponent(MeshType::PRIMITIVE_CUBE, false)
                    .Build();

				obj->Transform()->SetPosition(Vector3(0.0f, 10.0f, 0.0f)); // Set initial position above ground
                GameObjectManager::GetInstance()->AddGameObject(obj);
            }

            if (ImGui::MenuItem("Dynamic Physics Cube x 16"))
            {
                for (int i = 0; i < 16; ++i)
                {
                    auto name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsCube");

                    auto obj = GameObjectBuilder()
                        .SetName(name)
                        .AddTransformComponent(name)
                        .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                        .AddPhysicsComponent(MeshType::PRIMITIVE_CUBE, false)
                        .Build();

                    obj->Transform()->SetPosition(Vector3(0.0f, 10.0f, 0.0f)); // Set initial position above ground
                    GameObjectManager::GetInstance()->AddGameObject(obj);
                }
            }

            if (ImGui::MenuItem("Static Physics Plane"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsPlane");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_PLANE, MaterialType::DEFAULT)
                    .AddPhysicsComponent(MeshType::PRIMITIVE_PLANE, true)
                    .Build();

                GameObjectManager::GetInstance()->AddGameObject(obj);
            }
            ImGui::EndMenu();
        }

        // -------------------- Custom Meshes --------------------
        if (ImGui::BeginMenu("Custom Meshes"))
        {
            if (ImGui::MenuItem("Teapot"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Teapot");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::UTAH_TEAPOT, MaterialType::BRICK_TEX)
                    .Build();

                GameObjectManager::GetInstance()->AddGameObject(obj);
            }

            if (ImGui::MenuItem("Bunny"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Bunny");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::STANFORD_BUNNY, MaterialType::DEFAULT)
                    .Build();

                GameObjectManager::GetInstance()->AddGameObject(obj);
            }

            if (ImGui::MenuItem("Armadillo"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Armadillo");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::STANFORD_ARMADILLO, MaterialType::DEFAULT)
                    .Build();

                GameObjectManager::GetInstance()->AddGameObject(obj);
            }
            ImGui::EndMenu();
        }

        // -------------------- Nested Objects --------------------
        if (ImGui::BeginMenu("Nested Objects"))
        {
            if (ImGui::MenuItem("Level 1"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Nested");

                auto parent = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                    .Build();

                GameObjectManager::GetInstance()->AddGameObject(parent);

				name = NameRegistry::GetInstance()->GenerateUniqueName("Nested Child");

				auto child = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
					.AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
					.Build();

				child->Transform()->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
                parent->AttachChild(child);
            }

			ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void Toolbar::FileMenu()
{
    /* File Menus*/
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene"))
        {
            GameObjectManager::GetInstance()->ClearAllObjects();
        }

        if(ImGui::MenuItem("Save Scene"))
        {
            EngineGUIManager::GetInstance()->GetUI(UINames::SAVE_SCENE)->SetVisible(true);
		}

        if (ImGui::MenuItem("Load Scene"))
        {
            EngineGUIManager::GetInstance()->GetUI(UINames::LOAD_SCENE)->SetVisible(true);
        }

        if (ImGui::MenuItem("Exit"))
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

void Toolbar::Windows()
{
    if (ImGui::BeginMenu("Windows"))
    {
        auto uiList = EngineGUIManager::GetInstance()->GetAllScreens();

        for (const auto& screen : uiList)
        {
            if (screen->GetName() == UINames::TOOLBAR) continue; //exclude self
			if (screen->GetName() == UINames::SAVE_SCENE) continue; //exclude save window
			if (screen->GetName() == UINames::LOAD_SCENE) continue; //exclude load window

            bool visible = screen->IsVisible();

            if (ImGui::MenuItem(screen->GetName().c_str(), nullptr, visible))
            {
                screen->SetVisible(!visible);
            }
        }

        ImGui::EndMenu();
    }
}

void Toolbar::UndoRedo()
{
    float windowWidth = ImGui::GetWindowWidth();

    ImGui::SameLine((windowWidth / 2.0f) - 60.0f);

    if (ImGui::Button("Undo"))
    {
        if (ActionHistory::GetInstance()->HasRemainingUndoActions())
        {
            auto action = ActionHistory::GetInstance()->UndoAction();
            GameObjectManager::GetInstance()->ApplyEditorAction(action);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Redo"))
    {
        if (ActionHistory::GetInstance()->HasRemainingRedoActions())
        {
            auto action = ActionHistory::GetInstance()->RedoAction();
            GameObjectManager::GetInstance()->ApplyEditorAction(action);
        }
    }
}
