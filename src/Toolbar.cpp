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
#include "GameEntity.h"

#include "NameRegistry.h"
#include "ActionHistory.h"
#include "EditorState.h" // <-- Required include for the editor state controls
#include "SceneStateManager.h"
#include "EditorAction.h"

Toolbar::Toolbar() : AUIScreen("TOOLBAR")
{
	
}

void Toolbar::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        // --- Left-aligned Menus ---
        FileMenu();
        GameObjects();
        Lighting();
        Windows();

        // --- Centered Playback Controls ---
        DrawPlaybackControls();

        // --- Right-aligned FPS Counter ---
        DrawRightSideInfo();
    }

    ImGui::EndMainMenuBar();
}
void Toolbar::DrawPlaybackControls()
{
    // Calculate the center of the viewport
    float viewportWidth = ImGui::GetMainViewport()->Size.x;
    float controlsWidth = 200.0f; // Increased width to accommodate Reset button
    ImGui::SetCursorPosX((viewportWidth - controlsWidth) * 0.5f);

    EditorState currentState = EditorStateManager::GetState();

    // --- Play / Pause Button ---
    if (currentState == EditorState::PLAY)
    {
        if (ImGui::Button("Pause"))
        {
            EditorStateManager::SetState(EditorState::PAUSED);
        }
    }
    else // State is EDIT or PAUSED
    {
        if (ImGui::Button("Play"))
        {
            EditorStateManager::SetState(EditorState::PLAY);
        }
    }

    ImGui::SameLine();

    // --- Reset Button ---
    // Only enable if we have a saved state (i.e., we've played before)
    if (!SceneStateManager::GetInstance()->HasSavedState())
        ImGui::BeginDisabled();

    if (ImGui::Button("Reset"))
    {
        EditorStateManager::ResetScene();
        // Optionally, you can also set state back to EDIT if currently playing
        if (currentState != EditorState::EDIT)
        {
            EditorStateManager::SetState(EditorState::EDIT);
        }
    }

    if (!SceneStateManager::GetInstance()->HasSavedState())
        ImGui::EndDisabled();

    ImGui::SameLine();

    // --- Time Step Button ---
    if (currentState != EditorState::PAUSED) ImGui::BeginDisabled();
    if (ImGui::Button("Time Step"))
    {
        EditorStateManager::RequestTimeStep();
    }
    if (currentState != EditorState::PAUSED) ImGui::EndDisabled();
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

                auto entity = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .Build();

                entity->Transform()->SetPosition(Vector3(0.0f, 10.0f, 0.0f)); // Set initial position before physics

                GameObjectBuilder()
                    .SetExisting(entity)
                    .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                    .AddPhysicsComponent(MeshType::PRIMITIVE_CUBE, false)
                    .Build();
				
                GameObjectManager::GetInstance()->AddGameObject(entity);
            }

            if (ImGui::MenuItem("Dynamic Physics Cube x 16"))
            {
                for (int i = 0; i < 16; ++i)
                {
                    auto name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsCube");

                    auto entity = GameObjectBuilder()
                        .SetName(name)
                        .AddTransformComponent(name)
                        .Build();

                    entity->Transform()->SetPosition(Vector3(0.0f, 10.0f, 0.0f)); // Set initial position before physics

                    GameObjectBuilder()
                        .SetExisting(entity)
                        .AddMeshComponent(MeshType::PRIMITIVE_CUBE, MaterialType::DEFAULT)
                        .AddPhysicsComponent(MeshType::PRIMITIVE_CUBE, false)
                        .Build();

                    GameObjectManager::GetInstance()->AddGameObject(entity);
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

            if (ImGui::MenuItem("Lucy"))
            {
                auto name = NameRegistry::GetInstance()->GenerateUniqueName("Lucy");

                auto obj = GameObjectBuilder()
                    .SetName(name)
                    .AddTransformComponent(name)
                    .AddMeshComponent(MeshType::LUCY, MaterialType::DEFAULT)
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

    // This positions the text on the far right of the menu bar
    float windowWidth = ImGui::GetWindowWidth();
    float textWidth = ImGui::CalcTextSize(fpsText.c_str()).x;
    ImGui::SetCursorPosX(windowWidth - textWidth - 20.0f);

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

void Toolbar::DrawRightSideInfo()
{
    float windowWidth = ImGui::GetWindowWidth();

    // Reserve space for Undo, Redo and FPS
    float fpsTextWidth = ImGui::CalcTextSize("FPS: 000").x;
    float buttonWidth = 60.0f; // Approx width of Undo/Redo buttons
    float spacing = 10.0f;

    float totalWidth = (buttonWidth * 2) + spacing + fpsTextWidth + 10.0f;
    ImGui::SetCursorPosX(windowWidth - totalWidth);

    // Undo Button
    if (ImGui::Button("Undo"))
    {
        bool actionApplied = false;

        while (ActionHistory::GetInstance()->HasRemainingUndoActions())
        {
            auto action = ActionHistory::GetInstance()->UndoAction();
            auto go = GameObjectManager::GetInstance()->FindObjectByName(action->GetOwnerName());

            if (!go) continue;

            auto transform = go->Transform();
            bool same = transform->GetLocalPosition() == action->GetStorePos() &&
                transform->GetLocalQuaternion() == action->GetStoredOrientation() &&
                transform->GetLocalScale() == action->GetStoredScale();

            if (same) continue;

            GameObjectManager::GetInstance()->ApplyEditorAction(action);
            actionApplied = true;
            break;
        }

        if (!actionApplied)
        {
            Debug::LogWarning("No undo actions remaining.");
        }
    }

    ImGui::SameLine();

    // Redo Button
    if (ImGui::Button("Redo"))
    {
        bool actionApplied = false;

        while (ActionHistory::GetInstance()->HasRemainingRedoActions())
        {
            auto action = ActionHistory::GetInstance()->RedoAction();
            auto go = GameObjectManager::GetInstance()->FindObjectByName(action->GetOwnerName());

            if (!go) continue;

            auto transform = go->Transform();
            bool same = transform->GetLocalPosition() == action->GetStorePos() &&
                transform->GetLocalQuaternion() == action->GetStoredOrientation() &&
                transform->GetLocalScale() == action->GetStoredScale();

            if (same) continue;

            GameObjectManager::GetInstance()->ApplyEditorAction(action);
            actionApplied = true;
            break;
        }

        if (!actionApplied)
        {
            Debug::LogWarning("No redo actions remaining.");
        }
    }

    ImGui::SameLine();

    // FPS Text
    int fps = EngineTime::GetFPS();
    std::string fpsText = "FPS: " + std::to_string(fps);
    ImGui::Text("%s", fpsText.c_str());
}
