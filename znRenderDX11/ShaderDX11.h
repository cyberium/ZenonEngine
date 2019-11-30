#pragma once

#include "ShaderInputLayoutDX11.h"

class OW_ENGINE_API ShaderDX11 : public ShaderBase, public std::enable_shared_from_this<ShaderDX11>
{
public:
	ShaderDX11(std::weak_ptr<IRenderDeviceDX11> RenderDevice);
	virtual ~ShaderDX11();

	// IShader
	bool LoadShaderFromString(ShaderType type, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout) override final;
	bool LoadShaderFromFile(ShaderType type, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout) override final;
	
	bool LoadInputLayoutFromReflector() override final;
	bool LoadInputLayoutFromD3DElement(const std::vector<D3DVERTEXELEMENT9>& declIn) override final;

	void Bind() const override final;
	void UnBind() const override final;

	void Dispatch(const glm::uvec3& numGroups) override final;

protected:

	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	std::weak_ptr<IRenderDeviceDX11>   m_RenderDevice;

	ATL::CComPtr<ID3D11Device2>        m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	ATL::CComPtr<ID3D11VertexShader>   m_pVertexShader;
	ATL::CComPtr<ID3D11HullShader>     m_pHullShader;
	ATL::CComPtr<ID3D11DomainShader>   m_pDomainShader;
	ATL::CComPtr<ID3D11GeometryShader> m_pGeometryShader;
	ATL::CComPtr<ID3D11PixelShader>    m_pPixelShader;
	ATL::CComPtr<ID3D11ComputeShader>  m_pComputeShader;

	ATL::CComPtr<ID3DBlob>             m_pShaderBlob;
};
