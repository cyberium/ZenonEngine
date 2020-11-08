#pragma once

// Forward BEGIN
struct IManager;
// Forward END

struct ZN_API IConsoleCommand
{
	virtual ~IConsoleCommand() {};

	virtual const std::string GetName() const = 0;
	virtual bool HasArgs() const = 0;
	virtual void Execute() const = 0;
	virtual void Execute(const std::string& Args) const = 0;
};

typedef std::vector<std::shared_ptr<IConsoleCommand>> ConsoleCommands;

struct ZN_API IConsole 
	: public IManager
{
	ZN_OBJECTCLASS(cConsole)
	virtual ~IConsole() {}

	virtual bool AddConsoleCommand(std::shared_ptr<IConsoleCommand> Command) = 0;
	virtual std::shared_ptr<IConsoleCommand> GetConsoleCommandByName(const std::string& _commandName) = 0;
	virtual ConsoleCommands GetConsoleCommandHelp(std::string _input) = 0;
	virtual bool ProcessConsoleCommand(std::string _line) = 0;
};