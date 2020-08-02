#include "stdafx.h"

// General
#include "DebugOutputConsole.h"

// Additional
#include <iostream>

DebugOutput_ConsoleWindows::DebugOutput_ConsoleWindows()
{
	m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	m_DefaultConsoleColor = ConsoleWindowsColor::GRAY;

}

DebugOutput_ConsoleWindows::~DebugOutput_ConsoleWindows()
{}

//

void DebugOutput_ConsoleWindows::Print(IDebugOutput::DebugMessageType Type, const std::string& FormattedMessage)
{
	// Set color
	unsigned short color;
	switch (Type)
	{
		case IDebugOutput::DebugMessageType::TYPE_INFO:
		{
			color = ConsoleWindowsColor::GRAY;
		}
		break;
		case IDebugOutput::DebugMessageType::TYPE_PRINT:
		{
			color = ConsoleWindowsColor::WHITE;
		}
		break;
		case IDebugOutput::DebugMessageType::TYPE_GREEN:
		{
			color = ConsoleWindowsColor::GREEN;
		}
		break;
		case IDebugOutput::DebugMessageType::TYPE_WARNING:
		{
			color = ConsoleWindowsColor::YELLOW;
		}
		break;
		case IDebugOutput::DebugMessageType::TYPE_ERROR:
		case IDebugOutput::DebugMessageType::TYPE_FATAL:
		{
			color = ConsoleWindowsColor::RED;
		}
		break;
	}

	// To VisualStudio debug
	OutputDebugStringA(FormattedMessage.c_str());
	OutputDebugStringA("\n");

	// To console
	SetConsoleTextAttribute(m_ConsoleHandle, color);
	std::cout << FormattedMessage << std::endl;
	SetConsoleTextAttribute(m_ConsoleHandle, m_DefaultConsoleColor);
}
