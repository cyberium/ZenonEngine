#include "stdafx.h"

// General
#include "Console.h"


void Test(std::vector<std::string>& _args)
{
	Log::Print("00000000Args size[%d]", _args.size());

	for (auto it : _args)
		Log::Print("000000000Arg [%s]", it.c_str());
}

void Test1(int _argInt)
{
	Log::Print("11111111Arg = [%d]", _argInt);
}

void Test11(uint32 _argInt)
{
	Log::Print("11111111Arg = [%d]", _argInt);
}

void Test2() { Log::Print("TEST2"); }

//

CConsole::CConsole(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CConsole::~CConsole()
{
	m_BaseManager.RemoveManager<IConsole>();
}

bool CConsole::AddConsoleCommand(std::shared_ptr<IConsoleCommand> Command)
{
	_ASSERT(Command != nullptr);
	_ASSERT(false == Command->GetName().empty());

	// Already exists
	for (auto it : m_ConsoleCommands)
	{
		if (it->GetName() == Command->GetName())
		{
			Log::Error("Command [%s] already exists.", Command->GetName().c_str());
			return false;
		}
	}

	m_ConsoleCommands.push_back(Command);
	Log::Print("Command [%s] added.", Command->GetName().c_str());

	return true;
}

std::shared_ptr<IConsoleCommand> CConsole::GetConsoleCommandByName(const std::string& _commandName)
{
	if (_commandName.empty())
	{
		return nullptr;
	}

	for (auto it : m_ConsoleCommands)
	{
		if (it->GetName() == _commandName)
		{
			return it;
		}
	}

	return nullptr;
}

//--

ConsoleCommands CConsole::GetConsoleCommandHelp(std::string Line)
{
	Line = Utils::ToLower(Line);

	if (Line.empty())
		return ConsoleCommands();

	ConsoleCommands commands;
	for (auto& it : m_ConsoleCommands)
	{
		auto consoleCommandName = it->GetName();

		consoleCommandName = consoleCommandName.substr(0, Line.length());

		if (consoleCommandName == Line)
		{
			commands.push_back(it);
		}

		if (commands.size() == consoleCommandHelpMaxSize)
		{
			break;
		}
	}

	return commands;
}

bool CConsole::ProcessConsoleCommand(std::string _line)
{
	Utils::ToLower(_line);

	if (_line.empty())
		return false;

	std::string command;
	std::string args;

	// Divide line to command and args by first finded space
	auto spacePos = _line.find_first_of(' ');
	if (spacePos != std::string::npos)
	{
		command = Utils::Trim(_line.substr(0, spacePos));
		args = Utils::Trim(_line.substr(spacePos + 1));
	}
	else
	{
		command = _line;
		args = "";
	}

	// Find command in array
	auto consoleCommand = GetConsoleCommandByName(command);
	if (consoleCommand == nullptr)
	{
		Log::Warn("Command '%s' not found.", command.c_str());
		return false;
	}

	// Execute
	if (consoleCommand->HasArgs())
	{
		consoleCommand->Execute(args);
	}
	else
	{
		consoleCommand->Execute();
	}

	return true;
}
