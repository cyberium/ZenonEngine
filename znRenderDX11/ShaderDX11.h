#pragma once

#include "ShaderInputLayoutDX11.h"

class ZN_API ShaderDX11 
	: public ShaderBase
	, public std::enable_shared_from_this<ShaderDX11>
{
public:
	ShaderDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~ShaderDX11();

	// IShader
	bool LoadFromFile(EShaderType type, std::string fileName, ShaderMacros shaderMacros, std::string entryPoint) override final;
	
	bool LoadInputLayoutFromReflector() override final;
	bool LoadInputLayoutFromCustomElements(const std::vector<SCustomInputElement>& declIn) override final;

	void Bind() const override final;
	void UnBind() const override final;

	void Dispatch(const glm::uvec3& numGroups) override final;

private:
	void Destroy();

private:
	ATL::CComPtr<ID3D11VertexShader>   m_pVertexShader;
	ATL::CComPtr<ID3D11HullShader>     m_pHullShader;
	ATL::CComPtr<ID3D11DomainShader>   m_pDomainShader;
	ATL::CComPtr<ID3D11GeometryShader> m_pGeometryShader;
	ATL::CComPtr<ID3D11PixelShader>    m_pPixelShader;
	ATL::CComPtr<ID3D11ComputeShader>  m_pComputeShader;

	ATL::CComPtr<ID3DBlob>             m_pShaderBlob;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
