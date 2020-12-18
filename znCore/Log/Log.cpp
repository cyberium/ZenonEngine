#include "stdafx.h"

// General
#include "Log.h"

// Additional
#include "DebugOutput/DebugOutputLog.h"

#include <cstdarg>
//#include <debugapi.h>

CLog* gLogInstance = nullptr;

CLog::CLog()
{
	//OutputDebugStringA("Log created\n");
	gLogInstance = this;

	AddDebugOutput(MakeShared(CDebugOutputLog));

	Log::Green("Log created");
	
}

CLog::~CLog()
{
	gLogInstance = nullptr;

	//OutputDebugStringA("Log destroyed.\n");
}



//
// ILog
//
void CLog::AddDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput)
{
	_ASSERT(DebugOutput != nullptr);
	if (std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), DebugOutput) != m_DebugOutputs.end())
		throw CException("Debug output already exists.");

	{
		std::lock_guard<std::mutex> lck(m_Mutex);
		for (size_t i = 0; i < m_Messages.size(); i++)
		{
			const auto& msg = m_Messages.at(i);
			DebugOutput->Print(msg.first, msg.second);
		}

		if (auto debugOutputInternal = std::dynamic_pointer_cast<IDebugOutputInternal>(DebugOutput))
			debugOutputInternal->SetLastPushedIndex(m_Messages.size() - 1);
	}

	m_DebugOutputs.push_back(DebugOutput);
}

void CLog::DeleteDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput)
{
	_ASSERT(DebugOutput != nullptr);
	const auto& it = std::find(m_DebugOutputs.begin(), m_DebugOutputs.end(), DebugOutput);
	if (it == m_DebugOutputs.end())
		throw CException("Debug output not found.");
	m_DebugOutputs.erase(it);
}

void CLog::PushAllMessages()
{
	for (const auto& debugOutput : m_DebugOutputs)
	{
		if (auto debugOutputInternal = std::dynamic_pointer_cast<IDebugOutputInternal>(debugOutput))
		{
			std::lock_guard<std::mutex> lck(m_Mutex);

			size_t lastPushedMessage = debugOutputInternal->GetLastPushedIndex();
			for (size_t i = lastPushedMessage; i < m_Messages.size(); i++)
			{
				const auto& msg = m_Messages.at(i);
				debugOutput->Print(msg.first, msg.second);
				lastPushedMessage++;
			}
			debugOutputInternal->SetLastPushedIndex(lastPushedMessage);
		}
	}
}


//
// Private
//
void CLog::PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType Type, const char* Message, va_list& VaList)
{
	std::lock_guard<std::mutex> lck(m_Mutex);

	int len = vsnprintf(NULL, 0, Message, VaList);
	if (len > 0)
	{
		std::string buff;
		buff.resize(len + 1);
		vsnprintf(&buff[0], len + 1, Message, VaList);
		buff.resize(len);

#ifdef _WINDOWS
		// To VisualStudio debug
		//OutputDebugStringA(buff.c_str());
		//OutputDebugStringA("\n");
#endif

		auto messageObj = std::make_pair(Type, buff.c_str());

		// Add to log history
		m_Messages.push_back(messageObj);

		// Push immediately
		for (const auto& it : m_DebugOutputs)
			if (std::dynamic_pointer_cast<IDebugOutputInternal>(it) == nullptr)
				it->Print(messageObj.first, messageObj.second);
	}
}



//---------------------------------------------
//-- Static Log
//---------------------------------------------

void Log::Info(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_INFO, Message, args);
	va_end(args);
}

void Log::Print(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_PRINT, Message,  args);
	va_end(args);
}

void Log::Green(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_GREEN, Message,  args);
	va_end(args);
}

void Log::Warn(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_WARNING, Message,  args);
	va_end(args);
}

void Log::Error(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);
	if (gLogInstance)
		gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_ERROR, Message,  args);
	va_end(args);
}

void Log::Fatal(const char* Message, ...)
{
	va_list args;
	va_start(args, Message);

	// Push to common log
	{
		if (gLogInstance)
			gLogInstance->PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType::TYPE_ERROR, Message, args);
	}

	// And to message box
	/*{
		int len = vsnprintf(NULL, 0, Message, args);
		if (len > 0)
		{
			std::string buff;
			buff.resize(len + 1);
			vsnprintf(&buff[0], len + 1, Message, args);

			FatalMessageBox("Fatal error!", buff.c_str());
		}
	}*/
	va_end(args);
}