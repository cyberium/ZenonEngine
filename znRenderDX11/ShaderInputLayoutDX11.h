#pragma once

#include "ShaderInputSemanticDX11.h"

class ZN_API ShaderInputLayoutDX11 
	: public ShaderInputLayoutBase
{
public:
	ShaderInputLayoutDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~ShaderInputLayoutDX11();

    // IShaderInputLayout
    bool HasSemantic(const BufferBinding& binding) const override;
    const IShaderInputSemantic* GetSemantic(const BufferBinding& binding) const override;
    uint32 GetSemanticSlot(const BufferBinding& binding) const override;
	size_t GetSemanticsCount() const override;
	const IShaderInputSemantic* GetSemantic(size_t Index) const override;
	uint32 GetSemanticSlot(size_t Index) const override;

	// ShaderInputLayoutDX11
	void LoadFromReflector(std::shared_ptr<IByteBuffer> CompiledShaderBuffer);
	void LoadFromCustomElements(std::shared_ptr<IByteBuffer> CompiledShaderBuffer, const std::vector<SCustomInputElement>& CustomElements);
	ID3D11InputLayout* GetInputLayout() const;

private:
	std::map<UINT, std::shared_ptr<IShaderInputSemanticDX>> m_InputSemanticsDX11;
	ATL::CComPtr<ID3D11InputLayout> m_pInputLayout;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};