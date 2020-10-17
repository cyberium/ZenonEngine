#pragma once

class CShaderDX11Include
	: public ID3DInclude
{
public:
	CShaderDX11Include(IBaseManager& BaseManager);
	virtual ~CShaderDX11Include();

	// ID3DInclude
	HRESULT STDMETHODCALLTYPE Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes) override;
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData) override;

private:
	IBaseManager& m_BaseManager;
};
