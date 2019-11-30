#pragma once

#include "ConsoleCommand.h"

class OW_ENGINE_API CConsole : public IConsole
{
public:
	CConsole(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~CConsole();

	void AddCommonCommands();

	//IConsole
	bool AddConsoleCommand(IConsoleCommand* _command) override;
	IConsoleCommand* GetConsoleCommandByName(const std::string& _commandName) override;
	ConsoleCommands GetConsoleCommandHelp(std::string _input) override;
	bool ProcessConsoleCommand(std::string _line) override;

private:
	std::shared_ptr<IBaseManager> m_BaseManager;
	ConsoleCommands m_ConsoleCommands;

private: // static/const
	const unsigned consoleCommandHelpMaxSize = 7;
};
