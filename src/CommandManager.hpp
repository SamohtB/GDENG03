#pragma once
#include "ICommand.hpp"
#include "HotkeyListener.hpp"
#include <stack>

class CommandManager : public HotkeyListener
{
public:
    using CommandStack = std::stack<ICommand*>;

    static CommandManager* getInstance();
    static void initialize();
    static void destroy();

    void executeCommand(ICommand* command);
    void undo();
    void redo();

    void OnActionPressed(Hotkey::Action action) override
    {
        if (action == Hotkey::Action::Undo)
        {
            this->undo();
        }
        else if (action == Hotkey::Action::Redo)
        {
            this->redo();
        }
	}

private:
    CommandManager();
    ~CommandManager();
    CommandManager(const CommandManager&) = delete;
    CommandManager& operator=(const CommandManager&) = delete;

    static CommandManager* sharedInstance;

    void clearStack(CommandStack stack);

    CommandStack undoStack;
    CommandStack redoStack;
};
