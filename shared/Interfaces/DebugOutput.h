#pragma once

ZN_INTERFACE ZN_API IDebugOutput
{
	enum class ZN_API DebugMessageType
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