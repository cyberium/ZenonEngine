#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
// Forward END


ZN_INTERFACE ZN_API IDebugOutput
{
	enum ZN_API DebugMessageType
	{
		TYPE_INFO = 0,
		TYPE_PRINT,
		TYPE_GREEN,
		TYPE_WARNING,
		TYPE_ERROR
	};

	virtual ~IDebugOutput() {};

	virtual void Print(IDebugOutput::DebugMessageType Type, const std::string& Message) = 0;
};


ZN_INTERFACE ZN_API IDebugOutputInternal
{
	virtual ~IDebugOutputInternal() {};

	virtual void SetLastPushedIndex(size_t Index) = 0;
	virtual size_t GetLastPushedIndex() const = 0;
};


ZN_INTERFACE ZN_API ILog 
	: public IManager
{
	ZN_OBJECTCLASS(cLog)

	virtual ~ILog() {}

	virtual void AddDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) = 0;
	virtual void DeleteDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) = 0;
	virtual void PushAllMessages() = 0;
};
