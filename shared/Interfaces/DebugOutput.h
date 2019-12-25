#pragma once

struct ZN_API
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

	virtual ~IDebugOutput() {};

	virtual void Print(IDebugOutput::DebugMessageType Type, const std::string& FormattedMessage) = 0;
};