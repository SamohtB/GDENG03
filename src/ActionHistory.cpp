#include "pch.h"
#include "ActionHistory.h"
#include "EditorAction.h"
#include "EditorState.h"
#include "AGameObject.h"
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
    if (EditorStateManager::GetState() == EditorState::EDIT)
    {
        auto editorAction = new EditorAction(gameObject);
        this->m_actionsPerformed.push(editorAction);
		Debug::Log("ActionHistory::RecordAction: Recorded action for GameObject '" + gameObject->GetName() + "'");
    }
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
    if (EditorStateManager::GetState() != EditorState::EDIT)
    {
        Debug::LogWarning("Cannot perform any undo during play. \n");
        return nullptr;
    }

    auto action = this->m_actionsPerformed.top();
    this->m_actionsPerformed.pop();
    this->m_actionsCancelled.push(action);
    return action;
}

EditorAction* ActionHistory::RedoAction()
{
    if (EditorStateManager::GetState() != EditorState::EDIT)
    {
        Debug::LogWarning("Cannot perform any redo during play. \n");
        return nullptr;
    }

    auto action = this->m_actionsCancelled.top();
    this->m_actionsCancelled.pop();
    this->m_actionsPerformed.push(action);
    return action;
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


