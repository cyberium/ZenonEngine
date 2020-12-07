#pragma once

ZN_INTERFACE ZN_API IShaderInputSemanticDX
	: public virtual IShaderInputSemantic
{
	virtual ~IShaderInputSemanticDX() {}

	virtual DXGI_FORMAT GetDXFormat() const = 0;
};


class CShaderInputSemanticDX11
	: public ShaderInputSemanticBase
	, public IShaderInputSemanticDX
{
public:
	CShaderInputSemanticDX11(const std::string& Name, uint32 Index, DXGI_FORMAT DXFormat);
	virtual ~CShaderInputSemanticDX11();
	
	// IShaderInputSemantic
	bool IsEqual(const IShaderInputSemantic* Other) const override;

	// IShaderInputSemanticDX
	DXGI_FORMAT GetDXFormat() const override;

private:
	DXGI_FORMAT m_DXFormat;
};