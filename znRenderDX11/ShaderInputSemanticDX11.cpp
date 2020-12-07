#include "stdafx.h"

// General
#include "ShaderInputSemanticDX11.h"

CShaderInputSemanticDX11::CShaderInputSemanticDX11(const std::string & Name, uint32 Index, DXGI_FORMAT DXFormat)
	: ShaderInputSemanticBase(Name, Index)
	, m_DXFormat(DXFormat)
{
}

CShaderInputSemanticDX11::~CShaderInputSemanticDX11()
{
}



//
// IShaderInputSemantic
//
bool CShaderInputSemanticDX11::IsEqual(const IShaderInputSemantic * Other) const
{
	if (auto OtherAsDX11 = dynamic_cast<const IShaderInputSemanticDX*>(Other))
		return __super::IsEqual(Other) && GetDXFormat() == OtherAsDX11->GetDXFormat();
	return __super::IsEqual(Other);
}



//
// IShaderInputSemanticDX
//
DXGI_FORMAT CShaderInputSemanticDX11::GetDXFormat() const
{
	return m_DXFormat;
}
