#include "stdafx.h"

void DoCheckHR(HRESULT hr, const char* file, int line, const char* function, const wchar_t* Expr, const wchar_t* Message)
{
	_ASSERT(FAILED(hr));

	std::wstring fileUtf16 = Resources::utf8_to_utf16(file);
	std::wstring functionUtf16 = Resources::utf8_to_utf16(function);

	wchar_t modulePath[MAX_PATH];
	::GetModuleFileNameW(NULL, modulePath, MAX_PATH);

	const wchar_t* format = L"ZenonEngine HRESULT Failed!\r\n\r\n"
		L"Program: %s\r\n"
		L"File : %s\r\n"
		L"Line : %d\r\n"
		L"Funciton : %s\r\n"
		L"HRESULT : 0x%X\r\n\r\n"

		L"Message : %s\r\n"
		L"Expression : %s\r\n";

	throw CznRenderException(format, modulePath, fileUtf16.c_str(), line, functionUtf16.c_str(), hr, Message, Expr);

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