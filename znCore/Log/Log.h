#pragma once

#include <mutex>

class ZN_API CLog 
	: public ILog
{
	friend class Log;
public:
    CLog();
	virtual ~CLog();

	// ILog
	void AddDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) override;
	void DeleteDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) override;
	void PushAllMessages() override;

private:
    void PushMessageToAllDebugOutputs(IDebugOutput::DebugMessageType Type, const char* Message, va_list& VaList);

private:
	std::vector<std::pair<IDebugOutput::DebugMessageType, std::string>> m_Messages;
	//size_t m_LastPushedMessageIndex;
	std::vector<std::shared_ptr<IDebugOutput>> m_DebugOutputs;
	std::mutex m_Mutex;
};

// Helper class to fast access

class ZN_API Log
{
public:
	static void Info(const char* _message, ...);
	static void Print(const char* _message, ...);
	static void Green(const char* _message, ...);
	static void Warn(const char* _message, ...);
	static void Error(const char* _message, ...);
	static void Fatal(const char* _message, ...);
};