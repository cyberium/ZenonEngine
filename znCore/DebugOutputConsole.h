#pragma once

class DebugOutput_ConsoleWindows : public IDebugOutput
{
public:
	DebugOutput_ConsoleWindows();
	virtual ~DebugOutput_ConsoleWindows();

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& FormattedMessage) override;

private:
	enum ConsoleWindowsColor
	{
		GRAY = 8,
		BLUE = 9,
		GREEN = 10,
		LIGHTBLUE = 11,
		RED = 12,
		PINK = 13,
		YELLOW = 14,
		WHITE = 15
	};

	HANDLE m_ConsoleHandle;
	uint16 m_DefaultConsoleColor;
};
