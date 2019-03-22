#include "stdafx.h"

// General
#include "Console.h"

// Additional
#include "BaseManager.h"

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

CConsole::CConsole()
{
}

CConsole::~CConsole()
{
	ERASE_VECTOR(m_ConsoleCommands);

	DelManager<IConsole>();
}

void CConsole::AddCommonCommands()
{
	//ADDCONSOLECOMMAND_WITHARGS("test1_with_1", Test1, int32);
	//ADDCONSOLECOMMAND_WITHARGS("test11_with_1", Test11, uint32);
	//ADDCONSOLECOMMAND("test2_with_0", Test2);
}

bool CConsole::AddConsoleCommand(IConsoleCommand* _command)
{
	assert1(_command != nullptr);

	// Name is empty
	if (_command->GetName().empty())
	{
		Log::Error("Command name is empty.");
		return false;
	}

	// Already exists
	for (auto it : m_ConsoleCommands)
	{
		if (it->GetName() == _command->GetName())
		{
			Log::Error("Command [%s] already exists.", _command->GetName().c_str());
			return false;
		}
	}

	m_ConsoleCommands.push_back(_command);
	Log::Print("Command [%s] added.", _command->GetName().c_str());

	return true;
}

IConsoleCommand* CConsole::GetConsoleCommandByName(cstring _commandName)
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

ConsoleCommands CConsole::GetConsoleCommandHelp(std::string _input)
{
	Utils::ToLower(_input);

	if (_input.empty())
	{
		return ConsoleCommands();
	}

	ConsoleCommands commands;
	for (auto& it : m_ConsoleCommands)
	{
		auto consoleCommandName = it->GetName();

		consoleCommandName = consoleCommandName.substr(0, _input.length());

		if (consoleCommandName == _input)
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
	{
		return false;
	}

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
	IConsoleCommand* consoleCommand = GetConsoleCommandByName(command);

	if (consoleCommand == nullptr)
	{
		Log::Warn("Command [%s] not found.", command.c_str());
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
