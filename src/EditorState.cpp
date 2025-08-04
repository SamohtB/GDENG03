#include "pch.h"
#include "EditorState.h"
#include "GameObjectManager.h" // Required for ResetScene
#include "PhysicsSystem.h"     // Include PhysicsSystem
#include "PhysicsComponent.h"  // Include PhysicsComponent

// Initialize static variables
EditorState EditorStateManager::currentState = EditorState::EDIT;
bool EditorStateManager::timeStepRequested = false;

void EditorStateManager::Initialize()
{
    currentState = EditorState::EDIT;
    timeStepRequested = false;
}

void EditorStateManager::SetState(EditorState newState)
{
    // If we are not in PLAY mode and are about to enter PLAY mode, sync transforms.
    // This now correctly handles both EDIT -> PLAY and PAUSED -> PLAY transitions.
    if ((currentState == EditorState::EDIT || currentState == EditorState::PAUSED) && newState == EditorState::PLAY)
    {
        auto physicsSystem = PhysicsSystem::GetInstance();
        if (physicsSystem)
        {
            auto allComponents = physicsSystem->GetAllComponents();
            for (const auto& componentPtr : allComponents)
            {
                // This function ensures the Rigidbody's transform matches the GameObject's
                // transform at the moment the simulation (re)starts.
                componentPtr->UpdateTransformFromOwner();
            }
        }
    }

    currentState = newState;
}

EditorState EditorStateManager::GetState()
{
    return currentState;
}

void EditorStateManager::RequestTimeStep()
{
    if (currentState == EditorState::PAUSED)
    {
        timeStepRequested = true;
    }
}

bool EditorStateManager::IsTimeStepRequested()
{
    if (timeStepRequested)
    {
        timeStepRequested = false; // Consume the request
        return true;
    }
    return false;
}

void EditorStateManager::ResetScene()
{
    // This function is now only called manually by the "Reset" button.
    GameObjectManager::GetInstance()->ClearAllObjects();

    // TODO: Add any other necessary reset logic here (e.g., reloading from a file).
}