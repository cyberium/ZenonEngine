#pragma once

struct ZN_API IConsoleCommand
{
	virtual ~IConsoleCommand() {};

	virtual const std::string GetName() const = 0;
	virtual bool HasArgs() const = 0;
	virtual void Execute() = 0;
	virtual void Execute(const std::string& _args) = 0;
};

// Forward BEGIN
struct IManager;
// Forward END

typedef std::vector<IConsoleCommand*> ConsoleCommands;

struct ZN_API IConsole 
	: public IManager
{
	ZN_OBJECTCLASS(cConsole)

	virtual ~IConsole() = 0 {};

	virtual bool AddConsoleCommand(IConsoleCommand* _command) = 0;
	virtual IConsoleCommand* GetConsoleCommandByName(const std::string& _commandName) = 0;
	virtual ConsoleCommands GetConsoleCommandHelp(std::string _input) = 0;
	virtual bool ProcessConsoleCommand(std::string _line) = 0;
};