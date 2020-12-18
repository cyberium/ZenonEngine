#pragma once

class ZN_API CDebugOutputConsole_PlatformWindows
	: public IDebugOutput
{
public:
	CDebugOutputConsole_PlatformWindows();
	virtual ~CDebugOutputConsole_PlatformWindows();

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& Message) override;

private:
	HANDLE m_ConsoleHandle;
	uint16 m_DefaultConsoleColor;
};
