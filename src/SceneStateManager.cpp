
// ================================
// SceneStateManager.cpp
// ================================
#include "pch.h"
#include "SceneStateManager.h"
#include "AGameObject.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "GameObjectManager.h"
#include "Debug.h"

std::unique_ptr<SceneStateManager> SceneStateManager::sharedInstance = nullptr;

SceneStateManager* SceneStateManager::GetInstance()
{
    return sharedInstance.get();
}

void SceneStateManager::Initialize()
{
    if (!sharedInstance)
    {
        sharedInstance = std::make_unique<SceneStateManager>();
    }
}

void SceneStateManager::Destroy()
{
    sharedInstance.reset();
}

void SceneStateManager::SaveSceneState()
{
    m_savedStates.clear();

    auto allObjects = GameObjectManager::GetInstance()->GetAllObjects();

    for (AGameObject* gameObject : allObjects)
    {
        if (gameObject)
        {
            GameObjectState state = CaptureGameObjectState(gameObject);
            m_savedStates[gameObject->GetId()] = state;
        }
    }

    m_hasValidState = true;
    Debug::Log("Scene state saved for " + std::to_string(m_savedStates.size()) + " objects");
}

void SceneStateManager::RestoreSceneState()
{
    if (!m_hasValidState)
    {
        Debug::LogWarning("No saved scene state to restore");
        return;
    }

    auto allObjects = GameObjectManager::GetInstance()->GetAllObjects();

    for (AGameObject* gameObject : allObjects)
    {
        if (gameObject)
        {
            auto it = m_savedStates.find(gameObject->GetId());
            if (it != m_savedStates.end())
            {
                RestoreGameObjectState(gameObject, it->second);
            }
        }
    }

    Debug::Log("Scene state restored for " + std::to_string(m_savedStates.size()) + " objects");
}

void SceneStateManager::ClearSavedState()
{
    m_savedStates.clear();
    m_hasValidState = false;
}

bool SceneStateManager::HasSavedState() const
{
    return m_hasValidState;
}

GameObjectState SceneStateManager::CaptureGameObjectState(AGameObject* gameObject)
{
    GameObjectState state;

    // Capture transform
    if (gameObject->Transform())
    {
        state.position = gameObject->Transform()->GetLocalPosition();
        state.rotation = gameObject->Transform()->GetLocalRotation(); // Use GetLocalRotation() from your TransformComponent
        state.scale = gameObject->Transform()->GetLocalScale();
    }

    // Capture active state
    state.active = gameObject->IsActive();

    // Capture physics state if physics component exists
    PhysicsComponent* physicsComp = static_cast<PhysicsComponent*>(
        gameObject->FindComponentOfType(AComponent::ComponentType::Physics));

    if (physicsComp)
    {
        state.hasPhysics = true;
        auto rigidBody = physicsComp->GetRigidBody();
        if (rigidBody)
        {
            rp3d::Vector3 vel = rigidBody->getLinearVelocity();
            rp3d::Vector3 angVel = rigidBody->getAngularVelocity();

            state.velocity = Vector3(vel.x, vel.y, vel.z);
            state.angularVelocity = Vector3(angVel.x, angVel.y, angVel.z);
        }
    }
    else
    {
        state.hasPhysics = false;
        state.velocity = Vector3::Zero;
        state.angularVelocity = Vector3::Zero;
    }

    return state;
}

void SceneStateManager::RestoreGameObjectState(AGameObject* gameObject, const GameObjectState& state)
{
    // Restore transform
    if (gameObject->Transform())
    {
        gameObject->Transform()->SetPosition(state.position);
        gameObject->Transform()->SetRotation(state.rotation); // Use SetRotation(Vector3) from your TransformComponent
        gameObject->Transform()->SetScale(state.scale);
    }

    // Restore active state
    gameObject->SetActive(state.active);

    // Restore physics state if physics component exists
    if (state.hasPhysics)
    {
        PhysicsComponent* physicsComp = static_cast<PhysicsComponent*>(
            gameObject->FindComponentOfType(AComponent::ComponentType::Physics));

        if (physicsComp)
        {
            auto rigidBody = physicsComp->GetRigidBody();
            if (rigidBody)
            {
                // Reset velocities
                rigidBody->setLinearVelocity(rp3d::Vector3(
                    state.velocity.x, state.velocity.y, state.velocity.z));
                rigidBody->setAngularVelocity(rp3d::Vector3(
                    state.angularVelocity.x, state.angularVelocity.y, state.angularVelocity.z));

                // Update rigidbody transform to match GameObject transform
                physicsComp->UpdateTransformFromOwner();
            }
        }
    }
}