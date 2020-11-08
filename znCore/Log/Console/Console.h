#pragma once

#include "ConsoleCommand.h"

class ZN_API CConsole 
	: public IConsole
{
public:
	CConsole(IBaseManager& BaseManager);
	virtual ~CConsole();

	// IConsole
	bool AddConsoleCommand(std::shared_ptr<IConsoleCommand> Command) override;
	std::shared_ptr<IConsoleCommand> GetConsoleCommandByName(const std::string& _commandName) override;
	ConsoleCommands GetConsoleCommandHelp(std::string _input) override;
	bool ProcessConsoleCommand(std::string Line) override;

private:
	ConsoleCommands m_ConsoleCommands;

private:
	IBaseManager& m_BaseManager;

private:
	const unsigned consoleCommandHelpMaxSize = 7;
};
