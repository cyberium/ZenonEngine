#include "stdafx.h"

void DoCheckHR(HRESULT hr, ATL::CComBSTR Message)
{
	_ASSERT(FAILED(hr));

	ATL::CComPtr<IErrorInfo> errorInfo;
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

	throw CznRenderException(L"HRESULT 0x%H FAILED. Message: '%s'.", hr, description.m_str);
}