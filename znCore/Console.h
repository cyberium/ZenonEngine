#pragma once

#include "ConsoleCommand.h"

class CConsole : public IConsole
{
public:
	CConsole();
	virtual ~CConsole();

	void AddCommonCommands();

	//IConsole
	bool AddConsoleCommand(IConsoleCommand* _command) override;
	IConsoleCommand* GetConsoleCommandByName(cstring _commandName) override;
	ConsoleCommands GetConsoleCommandHelp(std::string _input) override;
	bool ProcessConsoleCommand(std::string _line) override;

private:
	ConsoleCommands m_ConsoleCommands;

private: // static/const
	const unsigned consoleCommandHelpMaxSize = 7;
};

// With args
//#define ADDCONSOLECOMMAND_WITHARGS(commandName, func, argType)                      GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_WA_Builder(func, argType)));
//#define ADDCONSOLECOMMAND_STATIC_WITHARGS(commandName, className, func, argType)    GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_STATIC_WA_Builder(className, func, argType)));
//#define ADDCONSOLECOMMAND_CLASS_WITHARGS(commandName, className, func, argType)     GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_CLASS_WA_Builder(className, this, func, argType)));

// Without args
//#define ADDCONSOLECOMMAND(commandName, func)                    GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_Builder(func)));
//#define ADDCONSOLECOMMAND_STATIÑ(commandName, className, func)  GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_STATIC_Builder(className, func)));
//#define ADDCONSOLECOMMAND_CLASS(commandName, className, func)   GetManager<IConsole>()->AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_CLASS_Builder(className, this, func)));