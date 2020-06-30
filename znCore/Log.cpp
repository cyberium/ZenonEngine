#include "stdafx.h"

// General
#include "Log.h"

// Additional
#include "DebugOutputConsole.h"
#include "DebugOutputLog.h"

CLog* gLogInstance = nullptr;

CLog::CLog()
{
	AddDebugOutput(std::make_shared<DebugOutput_ConsoleWindows>());
	AddDebugOutput(std::make_shared<DebugOutput_Log>());
	gLogInstance = this;

	OutputDebugString(L"Log created.\n");
}

CLog::~CLog()
{
	gLogInstance = nullptr;

	OutputDebugString(L"Log destroyed.\n");
}



//
// ILog
//
bool CLog::AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput)
{
	_ASSERT(_debugOutput != nullptr);

	if (std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput) != m_DebugOutputs.end())
		return false;

	for (const auto& messagePair : m_Messages)
	{
		PushMessageToDebugOutput(_debugOutput, messagePair.first, messagePair.second);
	}

	m_DebugOutputs.push_back(_debugOutput);

	return true;
}

bool CLog::DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput)
{
	_ASSERT(_debugOutput != nullptr);

	const auto& _debugOutputsIt = std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput);
	if (_debugOutputsIt == m_DebugOutputs.end())
		return false;

	m_DebugOutputs.erase(_debugOutputsIt);

	return true;
}


//
// Private
//
void CLog::PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList)
{
	std::lock_guard<std::mutex> lck(m_Mutex);

	int len = vsnprintf(NULL, 0, _message, _vaList);
	if (len > 0)
	{
		std::string buff;
		buff.resize(len + 1);
		vsnprintf(&buff[0], len + 1, _message, _vaList);

		// Add to log history
		m_Messages.push_back(std::make_pair(_type, buff));

		for (const auto& it : m_DebugOutputs)
		{
			PushMessageToDebugOutput(it, _type, buff);
		}
	}
}

void CLog::PushMessageToDebugOutput(const std::shared_ptr<IDebugOutput>& DebugOutput, IDebugOutput::DebugMessageType Type, const std::string& Message)
{
	std::string formattedMessage;

	// Set Prefix
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
	case IDebugOutput::DebugMessageType::TYPE_FATAL:
		formattedMessage = "FATAL ERROR: " + std::string(Message);
		break;
	}

	DebugOutput->Print(Type, formattedMessage);
}



//---------------------------------------------
//-- Static Log
//---------------------------------------------

void Log::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_INFO, _message, args);
	va_end(args);
}

void Log::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_PRINT, _message,  args);
	va_end(args);
}

void Log::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_GREEN, _message,  args);
	va_end(args);
}

void Log::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_WARNING, _message,  args);
	va_end(args);
}

void Log::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_ERROR, _message,  args);
	va_end(args);
}

void Log::Fatal(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	// Push to common log
	{
		if (gLogInstance)
			gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_ERROR, _message, args);
	}

	// And to message box
	{
		int len = vsnprintf(NULL, 0, _message, args);
		if (len > 0)
		{
			std::string buff;
			buff.resize(len + 1);
			vsnprintf(&buff[0], len + 1, _message, args);

			FatalMessageBox("Fatal error!", buff.c_str());
		}
	}
	va_end(args);
}