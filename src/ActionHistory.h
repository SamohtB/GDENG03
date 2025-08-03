#pragma once

class EditorAction;
class AGameObject;

class ActionHistory
{
public:
    typedef std::stack<EditorAction*> ActionStack;

    static ActionHistory* GetInstance();
    static void Initialize();
    static void Destroy();

    void RecordAction(AGameObject* gameObject);
    bool HasRemainingUndoActions();
    bool HasRemainingRedoActions();
    EditorAction* UndoAction();
    EditorAction* RedoAction();
    void Clear();

    ActionHistory();
    ~ActionHistory();
    ActionHistory(ActionHistory const&) = delete;
    ActionHistory& operator=(ActionHistory const&) = delete;
private:
    static std::unique_ptr<ActionHistory> sharedInstance;

    ActionStack m_actionsPerformed;
    ActionStack m_actionsCancelled;
};
