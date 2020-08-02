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

void DebugOutput_Log::Print(IDebugOutput::DebugMessageType Type, const std::string& FormattedMessage)
{
	m_LogStream << FormattedMessage << std::endl;
	m_LogStream.flush();
}
