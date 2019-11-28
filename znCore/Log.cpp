#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "Log.h"

// Additional
#include "BaseManager.h"
#include "DebugOutputConsole.h"
#include "DebugOutputLog.h"

std::shared_ptr<CLog> logInstance = nullptr;

CLog::CLog()
{
	m_DebugOutput_ConsoleWindows = std::make_shared<DebugOutput_ConsoleWindows>();
	AddDebugOutput(m_DebugOutput_ConsoleWindows);

	OutputDebugString(L"Log created.\n");

	logInstance = std::shared_ptr<CLog>(this);
}

CLog::~CLog()
{
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
	_ASSERT(_debugOutput != nullptr);

	if (std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput) != m_DebugOutputs.end())
	{
		return false;
	}

	m_DebugOutputs.push_back(_debugOutput);

	return true;
}

bool CLog::DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput)
{
	_ASSERT(_debugOutput != nullptr);

	auto _debugOutputsIt = std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), _debugOutput);

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

	for (const auto& it : m_DebugOutputs)
	{
		it->PushMessage(_type, _message, _vaList);
	}
}

//---------------------------------------------
//-- Static Log
//---------------------------------------------

void Log::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	logInstance->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_INFO, args);
	va_end(args);
}

void Log::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	logInstance->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_PRINT, args);
	va_end(args);
}

void Log::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	logInstance->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_GREEN, args);
	va_end(args);
}

void Log::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	logInstance->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_WARNING, args);
	va_end(args);
}

void Log::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);
	logInstance->PushMessageToAllDebugOutputs(_message, CDebugOutput::DebugMessageType::TYPE_ERROR, args);
	va_end(args);
}

void Log::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}