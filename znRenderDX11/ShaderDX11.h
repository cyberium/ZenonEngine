#pragma once

#include "ShaderInputLayoutDX11.h"

class ShaderDX11 : public Shader, public std::enable_shared_from_this<ShaderDX11>
{
public:
	ShaderDX11(ID3D11Device2* pDevice);
	virtual ~ShaderDX11();

	// IShader
	bool LoadShaderFromString(ShaderType type, cstring fileName, cstring source, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile, std::shared_ptr<IShaderInputLayout> _customLayout) override final;
	bool LoadShaderFromFile(ShaderType type, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile, std::shared_ptr<IShaderInputLayout> _customLayout) override final;
	
	bool LoadInputLayoutFromReflector() override final;
	bool LoadInputLayoutFromD3DElement(const std::vector<D3DVERTEXELEMENT9>& declIn) override final;

	std::shared_ptr<IShaderInputLayout> GetInputLayout() const override final;
	ShaderParameter& GetShaderParameterByName(cstring name) const override final;

	void Bind() override final;
	void UnBind() override final;

	void Dispatch(const glm::uvec3& numGroups) override final;

protected:

	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	ATL::CComPtr<ID3D11Device2>        m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	ATL::CComPtr<ID3D11VertexShader>   m_pVertexShader;
	ATL::CComPtr<ID3D11HullShader>     m_pHullShader;
	ATL::CComPtr<ID3D11DomainShader>   m_pDomainShader;
	ATL::CComPtr<ID3D11GeometryShader> m_pGeometryShader;
	ATL::CComPtr<ID3D11PixelShader>    m_pPixelShader;
	ATL::CComPtr<ID3D11ComputeShader>  m_pComputeShader;

	ATL::CComPtr<ID3DBlob>             m_pShaderBlob;


	std::shared_ptr<ShaderInputLayoutDX11> m_InputLayout;
	
	// Parameters necessary to reload the shader at runtime if it is modified on disc.
	ShaderMacros m_ShaderMacros;
	std::string m_EntryPoint;
	std::string m_Profile;
	std::string m_ShaderFileName;
};
