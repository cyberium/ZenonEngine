#pragma once

class ShaderInputLayoutDX11 : public IShaderInputLayout
{
public:
	ShaderInputLayoutDX11(ID3D11Device2* pDevice);
	virtual ~ShaderInputLayoutDX11();

	// IShaderInputLayout
	bool                 HasSemantic(const BufferBinding& binding) const override;
	const InputSemantic& GetSemantic(const BufferBinding& binding) const override;
	UINT                 GetSemanticSlot(const BufferBinding& binding) const override;

	// ShaderInputLayoutDX11
	bool                 LoadFromReflector(ID3DBlob * pShaderBlob, ID3D11ShaderReflection * pReflector);
	bool                 LoadFromD3D9(ID3DBlob * pShaderBlob, const std::vector<D3DVERTEXELEMENT9>& declIn);
	ID3D11InputLayout*   GetInputLayout() const;

private:
	ATL::CComPtr<ID3D11Device2>         m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>  m_pDeviceContext;

	ATL::CComPtr<ID3D11InputLayout>     m_pInputLayout;
};