#include "pch.h"
#include "EditorState.h"
#include "GameObjectManager.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "SceneStateManager.h" // Add this include
#include "Debug.h"

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
    EditorState previousState = currentState;

    // Handle state transitions
    if (previousState == EditorState::EDIT && newState == EditorState::PLAY)
    {
        // EDIT -> PLAY: Save current state and start simulation
        SceneStateManager::GetInstance()->SaveSceneState();

        // Sync transforms to physics bodies
        auto physicsSystem = PhysicsSystem::GetInstance();
        if (physicsSystem)
        {
            auto allComponents = physicsSystem->GetAllComponents();
            for (const auto& componentPtr : allComponents)
            {
                componentPtr->UpdateTransformFromOwner();
            }
        }

        Debug::Log("Entered PLAY mode - scene state saved");
    }
    else if (previousState == EditorState::PAUSED && newState == EditorState::PLAY)
    {
        // PAUSED -> PLAY: Resume simulation
        auto physicsSystem = PhysicsSystem::GetInstance();
        if (physicsSystem)
        {
            auto allComponents = physicsSystem->GetAllComponents();
            for (const auto& componentPtr : allComponents)
            {
                componentPtr->UpdateTransformFromOwner();
            }
        }

        Debug::Log("Resumed PLAY mode");
    }
    else if ((previousState == EditorState::PLAY || previousState == EditorState::PAUSED) && newState == EditorState::EDIT)
    {
        // PLAY/PAUSED -> EDIT: Stop simulation and restore original state
        SceneStateManager::GetInstance()->RestoreSceneState();
        Debug::Log("Stopped simulation - scene state restored");
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

// Manual reset function - can be called by a Reset button
void EditorStateManager::ResetScene()
{
    if (SceneStateManager::GetInstance()->HasSavedState())
    {
        SceneStateManager::GetInstance()->RestoreSceneState();
        Debug::Log("Scene manually reset to saved state");
    }
    else
    {
        Debug::LogWarning("No saved state available for reset");
    }
}