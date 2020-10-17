#include "stdafx.h"

// General
#include "ShaderDX11Include.h"

CShaderDX11Include::CShaderDX11Include(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{}

CShaderDX11Include::~CShaderDX11Include()
{}

HRESULT CShaderDX11Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes)
{
	_ASSERT(IncludeType == D3D_INCLUDE_LOCAL || IncludeType == D3D_INCLUDE_SYSTEM);
	try
	{
		std::shared_ptr<IFile> file = m_BaseManager.GetManager<IFilesManager>()->Open(std::string("shaders/") + pFileName);
		if (file == nullptr)
			throw CException(L"CShaderDX11Include: File '%s' not found.", pFileName);

		size_t fileSize = file->getSize();
		if (fileSize > 0)
		{
			char* buf = new char[fileSize];
			memcpy_s(buf, fileSize, file->getData(), fileSize);

			*ppData = buf;
			*pBytes = fileSize;
		}
		else
		{
			*ppData = nullptr;
			*pBytes = 0;
		}

		return S_OK;
	}
	catch (const CException& e)
	{
		ATL::CComPtr<ICreateErrorInfo> createErrorInfo;
		CHECK_HR(CreateErrorInfo(&createErrorInfo));

		ATL::CComBSTR description(Resources::utf8_to_utf16(e.Message()).c_str());
		CHECK_HR(createErrorInfo->SetDescription(description));

		ATL::CComPtr<IErrorInfo> errorInfo;
		CHECK_HR(createErrorInfo->QueryInterface(IID_IErrorInfo, (void**)&errorInfo));

		CHECK_HR(SetErrorInfo(0, errorInfo));

		return E_FAIL;
	}

	return E_ABORT;
}

HRESULT CShaderDX11Include::Close(LPCVOID pData)
{
	char* buf = (char*)pData;
	delete[] buf;
	return S_OK;
}