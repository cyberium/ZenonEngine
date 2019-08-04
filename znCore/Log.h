#pragma once

#include <mutex>

class CLog : public ILog
{
	friend class Log;
public:
    CLog();
	~CLog();

    void Info(const char* _message, ...);
    void Print(const char* _message, ...);
    void Green(const char* _message, ...);
    void Warn(const char* _message, ...);
    void Error(const char* _message, ...);
    void Fatal(const char* _message, ...);

	// ILog
	bool AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) override;
	bool DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) override;

private:
    void PushMessageToAllDebugOutputs(const char* _message, IDebugOutput::DebugMessageType _type, va_list& _vaList);

private:
	std::shared_ptr<IDebugOutput> m_DebugOutput_ConsoleWindows;

	std::vector<std::shared_ptr<IDebugOutput>> m_DebugOutputs;
	std::mutex m_Mutex;
};

// Helper class to fast access

class Log
{
public:
	static void Info(const char* _message, ...);
	static void Print(const char* _message, ...);
	static void Green(const char* _message, ...);
	static void Warn(const char* _message, ...);
	static void Error(const char* _message, ...);
	static void Fatal(const char* _message, ...);
};