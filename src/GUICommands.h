#pragma once
#include "ICommand.hpp"

class ModifyLayoutCommand : public ICommand
{
public:
	ModifyLayoutCommand(const std::string& beforeSnapshot, const std::string& afterSnapshot);
	~ModifyLayoutCommand() = default;

	void execute() override;
	void undo() override;

private:
	std::string m_before;
	std::string m_after;
};

