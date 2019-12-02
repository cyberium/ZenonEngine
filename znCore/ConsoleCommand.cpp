#include "stdafx.h"

// General
#include "ConsoleCommand.h"

ConsoleCommand::ConsoleCommand(const std::string& _commandName, const std::function<void()>& _function, bool _hasArgs) :
	m_Name(Utils::ToLower(_commandName)),
	m_Function(_function),
	m_HasArgs(_hasArgs)
{}

ConsoleCommand::~ConsoleCommand()
{}

const std::string ConsoleCommand::GetName() const 
{ 
	return m_Name; 
}

bool ConsoleCommand::HasArgs() const 
{ 
	return m_HasArgs; 
}

void ConsoleCommand::Execute()
{
	m_Function();
}

void ConsoleCommand::Execute(const std::string& _args)
{
	_ASSERT_EXPR(false, "Unable to call ConsoleCommand::Execute(const std::string& _args) in base class!");
}
