#pragma once

struct 
	__declspec(novtable, uuid("DBB1D38E-CE3F-4C05-8625-313BFC524C5A"))
	IDebugOutput
{
	enum DebugMessageType
	{
		TYPE_INFO = 0,
		TYPE_PRINT,
		TYPE_GREEN,
		TYPE_WARNING,
		TYPE_ERROR,
		TYPE_FATAL
	};

	virtual ~IDebugOutput() = 0 {};

	virtual void PushMessage(cstring _message, IDebugOutput::DebugMessageType _type) = 0;
	virtual void PushMessage(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList) = 0;
};