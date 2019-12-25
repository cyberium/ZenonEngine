#pragma once

#include "ConsoleCommand.h"

class ZN_API CConsole : public IConsole
{
public:
	CConsole(IBaseManager* BaseManager);
	virtual ~CConsole();

	void AddCommonCommands();

	//IConsole
	bool AddConsoleCommand(IConsoleCommand* _command) override;
	IConsoleCommand* GetConsoleCommandByName(const std::string& _commandName) override;
	ConsoleCommands GetConsoleCommandHelp(std::string _input) override;
	bool ProcessConsoleCommand(std::string _line) override;

private:
	IBaseManager* m_BaseManager;
	ConsoleCommands m_ConsoleCommands;

private:
	const unsigned consoleCommandHelpMaxSize = 7;
};
