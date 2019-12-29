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
		char* buff = new char[len + 1];
		vsnprintf(&buff[0], len + 1, Message, args);
		throw std::exception(buff);
		delete[] buff;
	}

	va_end(args);
}

CException::CException(std::string Message)
{
	throw std::exception(Message.c_str());
}

CException::CException(const wchar_t * WMessage, ...)
{
	va_list args;
	va_start(args, WMessage);

	int len = vswprintf(NULL, 0, WMessage, args);
	if (len > 0)
	{
		wchar_t* buff = new wchar_t[len + 1];
		vswprintf(&buff[0], len + 1, WMessage, args);
		throw std::exception(Resources::ConvertString(buff).c_str());
		delete[] buff;
	}

	va_end(args);
}

CException::CException(std::wstring WMessage)
{
	throw std::exception(Resources::ConvertString(WMessage).c_str());
}
