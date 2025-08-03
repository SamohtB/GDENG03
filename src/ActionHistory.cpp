#include "pch.h"
#include "ActionHistory.h"
#include "EditorAction.h"
#include "Debug.h"

std::unique_ptr<ActionHistory> ActionHistory::sharedInstance = nullptr;

ActionHistory* ActionHistory::GetInstance()
{
    return sharedInstance.get();
}

void ActionHistory::Initialize()
{
    sharedInstance = std::make_unique<ActionHistory>();
}

void ActionHistory::Destroy()
{
    sharedInstance.reset();
}

ActionHistory::ActionHistory()
{
}

ActionHistory::~ActionHistory()
{
    Clear();
}

void ActionHistory::RecordAction(AGameObject* gameObject)
{
    //if (StateManager::GetInstance()->GetMode() == StateManager::EditorMode::EDITOR)
    //{
        auto editorAction = new EditorAction(gameObject);
        this->m_actionsPerformed.push(editorAction);
    //}
}

bool ActionHistory::HasRemainingUndoActions()
{
    return !this->m_actionsPerformed.empty();
}

bool ActionHistory::HasRemainingRedoActions()
{
	return !this->m_actionsCancelled.empty();
}

EditorAction* ActionHistory::UndoAction()
{
    //if (StateManager::GetInstance()->GetMode() != StateManager::EditorMode::EDITOR)
    //{
    //    Debug::LogWarning("Cannot perform any undo during play. \n");
    //    return nullptr;
    //}

    if (this->HasRemainingUndoActions())
    {
        auto action = this->m_actionsPerformed.top();
        this->m_actionsPerformed.pop();
        this->m_actionsCancelled.push(action);
        return action;
    }

    else
    {
        Debug::LogWarning("No more actions remaining.");
        return nullptr;
    }
}

EditorAction* ActionHistory::RedoAction()
{
    //if (StateManager::GetInstance()->GetMode() != StateManager::EditorMode::EDITOR) 
    //{
    //    Debug::LogWarning("Cannot perform any redo during play. \n");
    //    return NULL;
    //}

    if (this->HasRemainingRedoActions()) 
    {
        auto action = this->m_actionsCancelled.top();
        this->m_actionsCancelled.pop();
        this->m_actionsPerformed.push(action);
        return action;
    }

    else 
    {
        Debug::LogWarning("No more actions remaining. \n");
        return NULL;
    }
}

void ActionHistory::Clear()
{
    while (this->HasRemainingUndoActions()) 
    {
        EditorAction* action = this->m_actionsPerformed.top();
        delete action;
        this->m_actionsPerformed.pop();
    }

    while (this->HasRemainingRedoActions()) 
    {
        EditorAction* action = this->m_actionsCancelled.top();
        delete action;
        this->m_actionsCancelled.pop();
    }
}


