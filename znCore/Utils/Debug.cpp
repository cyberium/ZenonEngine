#include "stdafx.h"

// General
#include "Debug.h"

// Additional
#include <cstdarg>


CException::CException(const char * Message, ...)
{
	va_list args;
	va_start(args, Message);

	int len = vsnprintf(NULL, 0, Message, args);
	if (len > 0)
	{
		m_Message.resize(len);
		vsnprintf(&m_Message[0], len, Message, args);
		m_Message.resize(len);
	}

	va_end(args);
}

CException::CException(std::string Message)
	: m_Message(Message)
{}

//--------------

CznRenderException::CznRenderException(const char * Message, ...)
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

CznRenderException::CznRenderException(std::string Message)
	: CException(Message) 
{}