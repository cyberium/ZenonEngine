#include "stdafx.h"

// General
#include "Debug.h"

CException::CException(const char * Message, ...)
{
	va_list args;
	va_start(args, Message);

	int len = vsnprintf(NULL, 0, Message, args);
	if (len > 0)
	{
		m_Message.resize(len);
		vsnprintf(&m_Message[0], len, Message, args);
	}

	va_end(args);
}

CException::CException(std::string Message)
	: m_Message(Message)
{
}

CException::CException(const wchar_t * WMessage, ...)
{
	va_list args;
	va_start(args, WMessage);

	int len = vswprintf(NULL, 0, WMessage, args);
	if (len > 0)
	{
		std::wstring wMessage;
		wMessage.resize(len);
		vswprintf(&wMessage[0], len, WMessage, args);
		m_Message = Resources::ConvertString(wMessage);
	}

	va_end(args);
}

CException::CException(std::wstring WMessage)
{
	m_Message = Resources::ConvertString(WMessage);
}
