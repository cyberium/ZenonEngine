#include "stdafx.h"

void DoCheckHR(HRESULT hr, const char* file, int line, const char* function, const wchar_t* Expr, const wchar_t* Message)
{
	_ASSERT(FAILED(hr));

	std::string exprUtf16 = Resources::utf16_to_utf8(Expr);
	std::string messageUtf16 = Resources::utf16_to_utf8(Message);

	wchar_t modulePath[MAX_PATH];
	::GetModuleFileNameW(NULL, modulePath, MAX_PATH);

	const char* format = "ZenonEngine HRESULT Failed!\r\n\r\n"
		"Program: %s\r\n"
		"File : %s\r\n"
		"Line : %d\r\n"
		"Funciton : %s\r\n"
		"HRESULT : 0x%X\r\n\r\n"

		"Message : %s\r\n"
		"Expression : %s\r\n";

	throw CznRenderException(format, modulePath, file, line, function, hr, messageUtf16.c_str(), exprUtf16.c_str());

	/*ATL::CComPtr<IErrorInfo> errorInfo;
	HRESULT _hr = ::GetErrorInfo(0, &errorInfo);
	if (FAILED(_hr))
		throw CznRenderException("Unable to obtain error info (0x%h).", hr);

	if (errorInfo == NULL)
		throw CznRenderException("Error info (0x%h) is NULL.", hr);

	ATL::CComBSTR description;
	if (FAILED(errorInfo->GetDescription(&description)))
		throw CznRenderException("Unable to read description from error info (0x%h).", hr);

	if (description.Length() == 0)
		throw CznRenderException("Description from error info (0x%h) is empty.", hr);

	if (Message.Length() > 0)
		throw CznRenderException(L"HRESULT 0x%H FAILED. Title: '%s'. Message: '%s'.", hr, Message.m_str, description.m_str);

	throw CznRenderException(L"HRESULT 0x%H FAILED. Message: '%s'.", hr, description.m_str);*/
}