#pragma once

#include "ShaderInputLayoutDX11.h"

class ZN_API ShaderDX11 : public ShaderBase, public std::enable_shared_from_this<ShaderDX11>
{
public:
	ShaderDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	virtual ~ShaderDX11();

	// IShader
	bool LoadShaderFromString(EShaderType type, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout) override final;
	bool LoadShaderFromFile(EShaderType type, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout) override final;
	
	bool LoadInputLayoutFromReflector() override final;
	bool LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn) override final;

	void Bind() const override final;
	void UnBind() const override final;

	void Dispatch(const glm::uvec3& numGroups) override final;

protected:

	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	ATL::CComPtr<ID3D11VertexShader>   m_pVertexShader;
	ATL::CComPtr<ID3D11HullShader>     m_pHullShader;
	ATL::CComPtr<ID3D11DomainShader>   m_pDomainShader;
	ATL::CComPtr<ID3D11GeometryShader> m_pGeometryShader;
	ATL::CComPtr<ID3D11PixelShader>    m_pPixelShader;
	ATL::CComPtr<ID3D11ComputeShader>  m_pComputeShader;

	ATL::CComPtr<ID3DBlob>             m_pShaderBlob;

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};
