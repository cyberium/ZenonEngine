#pragma once

#include "BlendStateDX11.h"
#include "RasterizerStateDX11.h"
#include "DepthStencilStateDX11.h"

class PipelineStateDX11 : public PipelineState
{
public:
	PipelineStateDX11(ID3D11Device2* pDevice);
	virtual ~PipelineStateDX11();

	// PipelineState
	void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader) override;
	std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const override;
	void SetBlendState(const BlendState& blendState) override;
	BlendState& GetBlendState() override;
	void SetRasterizerState(const RasterizerState& rasterizerState) override;
	RasterizerState& GetRasterizerState() override;
	void SetDepthStencilState(const DepthStencilState& depthStencilState) override;
	DepthStencilState& GetDepthStencilState() override;
	void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) override;
	std::shared_ptr<IRenderTarget> GetRenderTarget() const override;
	void Bind() override;
	void UnBind() override;

protected:

private:
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	ShaderMap m_Shaders;
	BlendStateDX11 m_BlendState;
	RasterizerStateDX11 m_RasterizerState;
	DepthStencilStateDX11 m_DepthStencilState;
	std::shared_ptr<IRenderTarget> m_RenderTarget;
};