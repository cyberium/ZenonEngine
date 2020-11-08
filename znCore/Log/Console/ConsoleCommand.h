#pragma once

#include <functional>

class ZN_API ConsoleCommand 
	: public IConsoleCommand
{
public:
	ConsoleCommand(const std::string& _commandName, const std::function<void()>& _function, bool _hasArgs = false);
	virtual ~ConsoleCommand();

	// IConsoleCommand
	const std::string GetName() const override;
	bool HasArgs() const override;
	void Execute() const override;
	void Execute(const std::string& Args) const override;

protected:
	std::string            m_Name;
	bool                   m_HasArgs;
    std::function<void()>  m_Function;
};
