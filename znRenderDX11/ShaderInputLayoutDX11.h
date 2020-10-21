#pragma once


struct InputSemanticDX11
	: InputSemantic
{
	InputSemanticDX11(const std::string& Name, uint32 Index, DXGI_FORMAT Format)
		: InputSemantic(Name, Index)
		, Format(Format)
	{}

	DXGI_FORMAT Format;

	inline bool operator==(const InputSemanticDX11& rhs) const
	{
		return __super::operator==(rhs) && (Format == rhs.Format);
	}
};


class ZN_API ShaderInputLayoutDX11 
	: public ShaderInputLayoutBase
{
public:
	ShaderInputLayoutDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~ShaderInputLayoutDX11();

    // IShaderInputLayout
    bool HasSemantic(const BufferBinding& binding) const override;
    const InputSemantic& GetSemantic(const BufferBinding& binding) const override;
    UINT GetSemanticSlot(const BufferBinding& binding) const override;

	// ShaderInputLayoutDX11
	bool LoadFromReflector(ID3DBlob * pShaderBlob, ID3D11ShaderReflection * pReflector);
	bool LoadFromCustomElements(ID3DBlob * pShaderBlob, const std::vector<SCustomInputElement>& CustomElements);
	ID3D11InputLayout* GetInputLayout() const;

private:
	std::map<UINT, InputSemanticDX11> m_InputSemanticsDX11;
	ATL::CComPtr<ID3D11InputLayout> m_pInputLayout;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};