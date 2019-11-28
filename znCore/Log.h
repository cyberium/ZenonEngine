#pragma once

#include <mutex>

class OW_ENGINE_API CLog : public ILog
{
	friend class Log;
public:
    CLog();
	~CLog();

	// ILog
	bool AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) override;
	bool DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) override;

private:
    void PushMessageToAllDebugOutputs(const char* _message, IDebugOutput::DebugMessageType _type, va_list& _vaList);

private:
	std::vector<std::shared_ptr<IDebugOutput>> m_DebugOutputs;
	std::mutex m_Mutex;
};

// Helper class to fast access

class OW_ENGINE_API Log
{
public:
	static void Info(const char* _message, ...);
	static void Print(const char* _message, ...);
	static void Green(const char* _message, ...);
	static void Warn(const char* _message, ...);
	static void Error(const char* _message, ...);
	static void Fatal(const char* _message, ...);
};