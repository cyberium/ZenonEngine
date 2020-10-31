#include "stdafx.h"

// General
#include "DebugOutputLog.h"

#define LOGFILENAME "log.txt"

DebugOutput_Log::DebugOutput_Log()
{
	std::string fileName = std::string(LOGFILENAME);

	if (fileName.empty())
	{
		Log::Error("LogFile[%s]: No such file or directory!", fileName.c_str());
		_ASSERT(false);
	}

	m_LogStream.open(fileName.c_str(), std::ios::out);
	if (!m_LogStream.is_open())
	{
		Log::Error("LogFile[%s]: Can not open file!", fileName.c_str());
		m_LogStream.clear();
		_ASSERT(false);
	}
}

DebugOutput_Log::~DebugOutput_Log()
{
	m_LogStream.flush();
	m_LogStream.close();
	m_LogStream.clear();
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
