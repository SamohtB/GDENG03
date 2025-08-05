#pragma once
#include "Math.h"
#include <unordered_map>
#include <memory>

class AGameObject;
class TransformComponent;

struct GameObjectState
{
    Vector3 position;
    Vector3 rotation; // Euler angles
    Vector3 scale;
    bool active;

    // Physics-specific state
    Vector3 velocity;
    Vector3 angularVelocity;
    bool hasPhysics;
};

class SceneStateManager
{
public:
    static SceneStateManager* GetInstance();
    static void Initialize();
    static void Destroy();

    // Save the current state of all GameObjects
    void SaveSceneState();

    // Restore all GameObjects to their saved state
    void RestoreSceneState();

    // Clear saved state (useful when scene changes in edit mode)
    void ClearSavedState();

    // Check if we have a saved state
    bool HasSavedState() const;

    // Constructor and destructor need to be public for std::make_unique
    SceneStateManager() = default;
    ~SceneStateManager() = default;

    // Delete copy constructor and assignment operator to maintain singleton
    SceneStateManager(const SceneStateManager&) = delete;
    SceneStateManager& operator=(const SceneStateManager&) = delete;

private:
    static std::unique_ptr<SceneStateManager> sharedInstance;

    std::unordered_map<int, GameObjectState> m_savedStates; // GameObject ID -> State
    bool m_hasValidState = false;

    GameObjectState CaptureGameObjectState(AGameObject* gameObject);
    void RestoreGameObjectState(AGameObject* gameObject, const GameObjectState& state);
};