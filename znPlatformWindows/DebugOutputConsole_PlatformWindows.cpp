#include "stdafx.h"

// General
#include "DebugOutputConsole_PlatformWindows.h"

// Additional
#include <iostream>

namespace
{
	enum ConsoleWindowsColor : uint16
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
}

CDebugOutputConsole_PlatformWindows::CDebugOutputConsole_PlatformWindows()
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_DefaultConsoleColor = ConsoleWindowsColor::GRAY;
}

CDebugOutputConsole_PlatformWindows::~CDebugOutputConsole_PlatformWindows()
{}



//
// IDebugOutput
//
void CDebugOutputConsole_PlatformWindows::Print(IDebugOutput::DebugMessageType Type, const std::string& Message)
{
	// Set color
	uint16 color;
	switch (Type)
	{
		case IDebugOutput::DebugMessageType::TYPE_INFO:
			color = ConsoleWindowsColor::GRAY;
			break;
		case IDebugOutput::DebugMessageType::TYPE_PRINT:
			color = ConsoleWindowsColor::WHITE;
			break;
		case IDebugOutput::DebugMessageType::TYPE_GREEN:
			color = ConsoleWindowsColor::GREEN;
			break;
		case IDebugOutput::DebugMessageType::TYPE_WARNING:
			color = ConsoleWindowsColor::YELLOW;
			break;
		case IDebugOutput::DebugMessageType::TYPE_ERROR:
			color = ConsoleWindowsColor::RED;
			break;
		default:
			_ASSERT(false);
	}

	// To console
	SetConsoleTextAttribute(m_ConsoleHandle, color);
	std::cout << Message << std::endl;
	SetConsoleTextAttribute(m_ConsoleHandle, m_DefaultConsoleColor);
}
