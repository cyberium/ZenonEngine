#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "Log.h"

// Additional
#include "BaseManager.h"
#include "DebugOutputConsole.h"
#include "DebugOutputLog.h"

CLog::CLog()
{
	m_DebugOutput_ConsoleWindows = std::make_shared<DebugOutput_ConsoleWindows>();
	AddDebugOutput(m_DebugOutput_ConsoleWindows);

	OutputDebugString(L"Log created.\n");
}

CLog::~CLog()
{
	DelManager<ILog>();

	OutputDebugString(L"Log destroyed.\n");
}

//--

void CLog::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_INFO, args);
	va_end(args);
}

void CLog::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_PRINT, args);
	va_end(args);
}

void CLog::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_GREEN, args);
	va_end(args);
}

void CLog::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_WARNING, args);
	va_end(args);
}

void CLog::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_ERROR, args);
	va_end(args);
}

void CLog::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}

//--

bool CLog::AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput)
{
	assert1(_debugOutput != nullptr);

	if (find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput) != m_DebugOutputs.end())
	{
		return false;
	}

	m_DebugOutputs.push_back(_debugOutput);

	return true;
}

bool CLog::DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput)
{
	assert1(_debugOutput != nullptr);

	auto _debugOutputsIt = find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput);

	// Not exists
	if (_debugOutputsIt == m_DebugOutputs.end())
	{
		return false;
	}

	m_DebugOutputs.erase(_debugOutputsIt);

	return true;
}

void CLog::PushMessageToAllDebugOutputs(const char* _message, IDebugOutput::DebugMessageType _type, va_list& _vaList)
{
	std::unique_lock<std::mutex> lck(m_Mutex, std::defer_lock);

	for (auto it : m_DebugOutputs)
	{
		it->PushMessage(_type, _message, _vaList);
	}
}

//---------------------------------------------
//-- Static Log
//---------------------------------------------

inline std::shared_ptr<CLog> getLog()
{
	return std::dynamic_pointer_cast<CLog, ILog>(GetManager<ILog>());
}

void Log::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_INFO, args);
	va_end(args);
}

void Log::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_PRINT, args);
	va_end(args);
}

void Log::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_GREEN, args);
	va_end(args);
}

void Log::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_WARNING, args);
	va_end(args);
}

void Log::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	getLog()->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_ERROR, args);
	va_end(args);
}

void Log::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}