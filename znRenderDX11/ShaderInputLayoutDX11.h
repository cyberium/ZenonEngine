#pragma once

class ZN_API ShaderInputLayoutDX11 
	: public ShaderInputLayoutBase
{
public:
	ShaderInputLayoutDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	virtual ~ShaderInputLayoutDX11();

    // IShaderInputLayout
    bool HasSemantic(const BufferBinding& binding) const override;
    const InputSemantic& GetSemantic(const BufferBinding& binding) const override;
    UINT GetSemanticSlot(const BufferBinding& binding) const override;

	// ShaderInputLayoutDX11
	bool LoadFromReflector(ID3DBlob * pShaderBlob, ID3D11ShaderReflection * pReflector);
	bool LoadFromCustomElements(ID3DBlob * pShaderBlob, const std::vector<SCustomVertexElement>& CustomElements);
	ID3D11InputLayout* GetInputLayout() const;

private:
	ATL::CComPtr<ID3D11InputLayout> m_pInputLayout;

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};