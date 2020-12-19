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
	bool LoadFromFile(EShaderType Type, std::string Filename, ShaderMacros Macroses, std::string EntryPoint) override final;
	
	bool LoadInputLayoutFromReflector() override final;
	bool LoadInputLayoutFromCustomElements(const std::vector<SCustomInputElement>& CustomElements) override final;

	void Bind() const override final;
	void UnBind() const override final;
	void Dispatch(const glm::uvec3& numGroups) override final;

private:
	std::shared_ptr<IByteBuffer> CompileShader(EShaderType Type, std::string Filename, ShaderMacros Macroses, std::string EntryPoint);
	void LoadCompiledShader(std::shared_ptr<IByteBuffer> ByteBuffer);
	void Destroy();

private:
	ATL::CComPtr<ID3D11VertexShader>   m_VertexShader;
	ATL::CComPtr<ID3D11HullShader>     m_HullShader;
	ATL::CComPtr<ID3D11DomainShader>   m_DomainShader;
	ATL::CComPtr<ID3D11GeometryShader> m_GeometryShader;
	ATL::CComPtr<ID3D11PixelShader>    m_PixelShader;
	ATL::CComPtr<ID3D11ComputeShader>  m_ComputeShader;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
