#include "stdafx.h"

// General
#include "DebugOutputLog.h"

#define LOGFILENAME "log.txt"

DebugOutput_Log::DebugOutput_Log()
{
	m_LogStream.open(LOGFILENAME, std::ios::out);
	if (false == m_LogStream.is_open())
		throw CException("Unable create log file in Application directory.");
}

DebugOutput_Log::~DebugOutput_Log()
{
	m_LogStream.flush();
	m_LogStream.close();
}

//

void DebugOutput_Log::Print(IDebugOutput::DebugMessageType Type, const std::string& Message)
{
	std::string formattedMessage;

	switch (Type)
	{
	case IDebugOutput::DebugMessageType::TYPE_INFO:
		formattedMessage = "~" + std::string(Message);
		break;
	case IDebugOutput::DebugMessageType::TYPE_PRINT:
		formattedMessage = " " + std::string(Message);
		break;
	case IDebugOutput::DebugMessageType::TYPE_GREEN:
		formattedMessage = "^" + std::string(Message);
		break;
	case IDebugOutput::DebugMessageType::TYPE_WARNING:
		formattedMessage = "@" + std::string(Message);
		break;
	case IDebugOutput::DebugMessageType::TYPE_ERROR:
		formattedMessage = "!" + std::string(Message);
		break;
	}

	m_LogStream << formattedMessage << std::endl;
	m_LogStream.flush();
}
