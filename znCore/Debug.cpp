#include "stdafx.h"

// General
#include "Debug.h"

CException::CException()
	: std::exception("Assertion failed!")
{}

CException::CException(std::string Message)
	: std::exception(Message.c_str())
{}

CException::CException(std::wstring Message)
	: std::exception(Resources::ConvertString(Message).c_str())
{}
