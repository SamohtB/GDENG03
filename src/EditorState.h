#pragma once

// The core states of the editor
enum class EditorState
{
    EDIT,
    PLAY,
    PAUSED
};

class EditorStateManager
{
public:
    static void Initialize();

    static void SetState(EditorState state);
    static EditorState GetState();

    // Call this to request a single frame update while paused
    static void RequestTimeStep();
    static bool IsTimeStepRequested();

    // Call this to reset the scene to its starting state
    static void ResetScene();

private:
    static EditorState currentState;
    static bool timeStepRequested;
};