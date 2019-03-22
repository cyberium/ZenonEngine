#include "stdafx.h"

// General
#include "DebugOutput.h"

void CDebugOutput::PushMessage(CDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList)
{
	// Parse args end
	int len = vsnprintf(NULL, 0, _message, _vaList);
	std::string buff;
	if (len > 0)
	{
		buff.resize(len + 1);
		vsnprintf(&buff[0], len + 1, _message, _vaList);
		PushMessage(buff, _type);
	}
}

void CDebugOutput::PushMessage(cstring _message, CDebugOutput::DebugMessageType _type)
{
	std::string messageWithPrefix;

	// Set Prefix
	switch (_type)
	{
		case TYPE_INFO:
		messageWithPrefix = "~" + std::string(_message);
		break;
		case TYPE_PRINT:
		messageWithPrefix = " " + std::string(_message);
		break;
		case TYPE_GREEN:
		messageWithPrefix = "^" + std::string(_message);
		break;
		case TYPE_WARNING:
		messageWithPrefix = "@" + std::string(_message);
		break;
		case TYPE_ERROR:
		messageWithPrefix = "!" + std::string(_message);
		break;
		case TYPE_FATAL:
		messageWithPrefix = "FATAL ERROR: " + std::string(_message);
		break;
	}

	Print(messageWithPrefix, _type);
}
