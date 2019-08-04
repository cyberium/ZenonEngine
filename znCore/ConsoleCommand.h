#pragma once

#include <functional>

class OW_ENGINE_API ConsoleCommand : public IConsoleCommand
{
public:
	ConsoleCommand(const std::string& _commandName, const std::function<void()>& _function, bool _hasArgs = false);
	virtual ~ConsoleCommand();

	// IConsoleCommand
	const std::string GetName() const override;
	bool HasArgs() const override;
	void Execute() override;
	void Execute(const std::string& _args) override;

protected:
	std::string     m_Name;
	bool       m_HasArgs;
    std::function<void()>  m_Function;
};

template <class ArgumentType>
class OW_ENGINE_API ConsoleCommand_WA : public ConsoleCommand
{
public:
	ConsoleCommand_WA(const std::string& _commandName, const std::function<void(ArgumentType)>& _function) :
		ConsoleCommand(_commandName, _function, true)
	{}
	virtual ~ConsoleCommand_WA()
	{}

	// IConsoleCommand
	const std::string GetName() const override { return ConsoleCommand::GetName(); };
	bool HasArgs() const override { return ConsoleCommand::HasArgs(); };
	void Execute() override { return ConsoleCommand::Execute(); };
	void Execute(const std::string& _args) override
	{
		ArgumentType value;

		if (Utils::TryParse(typeid(ArgumentType), _args, &value))
		{
			//Function_WA<ArgumentType>* funcWA = dynamic_cast<Function_WA<ArgumentType>*>(m_Function);
			//_ASSERT(funcWA != nullptr);

            m_Function();

			//funcWA->operator()(ArgumentType(value));
		}
		else
		{
			Log::Error("ConsoleCommand_WA[%s]: Can't parse argument [%s] to [%s].", m_Name.c_str(), _args.c_str(), typeid(ArgumentType).name());
		}
	}
};