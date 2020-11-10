#include "stdafx.h"

// General
#include "Log.h"

// Additional
#include "DebugOutput/DebugOutputConsole.h"
#include "DebugOutput/DebugOutputLog.h"

CLog* gLogInstance = nullptr;

CLog::CLog()
	: m_LastPushedMessageIndex(0)
{
	AddDebugOutput(MakeShared(DebugOutput_ConsoleWindows));
	AddDebugOutput(MakeShared(DebugOutput_Log));
	gLogInstance = this;

	OutputDebugString(L"Log created\n");
	Log::Green("Log created");
}

CLog::~CLog()
{
	gLogInstance = nullptr;
	OutputDebugString(L"Log destroyed.\n");
}



//
// ILog
//
bool CLog::AddDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput)
{
	_ASSERT(DebugOutput != nullptr);

	if (std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), DebugOutput) != m_DebugOutputs.end())
		return false;

	{
		std::lock_guard<std::mutex> lck(m_Mutex);
		if (m_LastPushedMessageIndex < m_Messages.size())
		{
			for (size_t i = 0; i < m_LastPushedMessageIndex; i++)
			{
				const auto& msg = m_Messages.at(i);
				DebugOutput->Print(msg.first, msg.second);
			}
		}
	}

	m_DebugOutputs.push_back(DebugOutput);
	return true;
}

bool CLog::DeleteDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput)
{
	_ASSERT(DebugOutput != nullptr);

	const auto& it = std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), DebugOutput);
	if (it == m_DebugOutputs.end())
		return false;

	m_DebugOutputs.erase(it);
	return true;
}

void CLog::PushAllMessages()
{
	std::lock_guard<std::mutex> lck(m_Mutex);

	for (size_t i = m_LastPushedMessageIndex; i < m_Messages.size(); i++)
	{
		const auto& msg = m_Messages.at(i);
		for (const auto& it : m_DebugOutputs)
			it->Print(msg.first, msg.second);
		m_LastPushedMessageIndex++;
	}
}


//
// Private
//
void CLog::PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType _type, const char* _message, va_list& VaList)
{
	std::lock_guard<std::mutex> lck(m_Mutex);

	int len = vsnprintf(NULL, 0, _message, VaList);
	if (len > 0)
	{
		std::string buff;
		buff.resize(len + 1);
		vsnprintf(&buff[0], len + 1, _message, VaList);
		buff.resize(len);

		// Add to log history
		m_Messages.push_back(std::make_pair(_type, buff.c_str()));
	}
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