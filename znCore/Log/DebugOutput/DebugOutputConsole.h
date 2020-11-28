#pragma once

class CDebugOutputConsole
	: public IDebugOutput
{
public:
	CDebugOutputConsole();
	virtual ~CDebugOutputConsole();

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& Message) override;

private:
	HANDLE m_ConsoleHandle;
	uint16 m_DefaultConsoleColor;
};
