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

void DebugOutput_ConsoleWindows::Print(std::string _messageFmt, CDebugOutput::DebugMessageType _type)
{
	// Set color
	unsigned short color;
	switch (_type)
	{
		case TYPE_INFO:
		color = ConsoleWindowsColor::GRAY;
		break;
		case TYPE_PRINT:
		color = ConsoleWindowsColor::WHITE;
		break;
		case TYPE_GREEN:
		color = ConsoleWindowsColor::GREEN;
		break;
		case TYPE_WARNING:
		color = ConsoleWindowsColor::YELLOW;
		break;
		case TYPE_ERROR:
		case TYPE_FATAL:
		color = ConsoleWindowsColor::RED;
		break;
	}

	// To VisualStudio debug
	OutputDebugStringA(_messageFmt.c_str());
	OutputDebugStringA("\n");

	// To console
	SetConsoleTextAttribute(m_ConsoleHandle, color);
	std::cout << _messageFmt << std::endl;
	SetConsoleTextAttribute(m_ConsoleHandle, m_DefaultConsoleColor);
}
