#pragma once

struct
	__declspec(novtable)
	IConsoleCommand
{
	virtual ~IConsoleCommand() {};

	virtual const std::string GetName() const = 0;
	virtual bool HasArgs() const = 0;
	virtual void Execute() = 0;
	virtual void Execute(cstring _args) = 0;
};

// Forward BEGIN
struct IManager;
// Forward END

typedef std::vector<IConsoleCommand*> ConsoleCommands;

struct 
	__declspec(novtable, uuid("1CB48B2B-357E-4B11-8587-3D6A8385A436"))
	IConsole : public IManager
{
	virtual ~IConsole() = 0 {};

	virtual bool AddConsoleCommand(IConsoleCommand* _command) = 0;
	virtual IConsoleCommand* GetConsoleCommandByName(cstring _commandName) = 0;
	virtual ConsoleCommands GetConsoleCommandHelp(std::string _input) = 0;
	virtual bool ProcessConsoleCommand(std::string _line) = 0;
};