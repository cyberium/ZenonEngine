#pragma once

#include "BlendStateDX11.h"
#include "RasterizerStateDX11.h"
#include "DepthStencilStateDX11.h"

class ZN_API PipelineStateDX11 : public IPipelineState
{
public:
	PipelineStateDX11(ID3D11Device2* pDevice);
	virtual ~PipelineStateDX11();

	// PipelineState
	void SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader) override;
	std::shared_ptr<IShader> GetShader(IShader::ShaderType type) const override;
	const ShaderMap& GetShaders() const override;
	void SetBlendState(const std::shared_ptr<IBlendState> blendState) override;
	std::shared_ptr<IBlendState> GetBlendState() const override;
	void SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState) override;
	std::shared_ptr<IRasterizerState> GetRasterizerState() const override;
	void SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState) override;
	std::shared_ptr<IDepthStencilState> GetDepthStencilState() const override;
	void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) override;
	std::shared_ptr<IRenderTarget> GetRenderTarget() const override;

	void Bind() override;
	void UnBind() override;

protected:

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;

	ShaderMap                                       m_Shaders;
	std::shared_ptr<BlendStateDX11>                 m_BlendState;
	std::shared_ptr<RasterizerStateDX11>            m_RasterizerState;
	std::shared_ptr<DepthStencilStateDX11>          m_DepthStencilState;
	std::shared_ptr<IRenderTarget>                  m_RenderTarget;
};