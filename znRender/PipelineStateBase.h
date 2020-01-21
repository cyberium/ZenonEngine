#pragma once

class ZN_API PipelineStateBase
	: public IPipelineState
	, public Object
{
public:
	PipelineStateBase();
	virtual ~PipelineStateBase();

	// IPipelineState
	void SetShader(EShaderType type, IShader* pShader) override;
	IShader* GetShader(EShaderType type) const override;
	const ShaderMap& GetShaders() const override;

	void SetTexture(uint8 ID, ITexture* texture) override;
	ITexture* GetTexture(uint8 ID) const override;
	const TextureMap& GetTextures() const override;

	void SetSampler(uint8 ID, ISamplerState* samplerState) override;
	ISamplerState* GetSampler(uint8 ID) const override;
	const SamplersMap& GetSamplers() const override;

	void SetBlendState(const std::shared_ptr<IBlendState> blendState) override;
	std::shared_ptr<IBlendState> GetBlendState() const override;

	void SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState) override;
	std::shared_ptr<IRasterizerState> GetRasterizerState() const override;

	void SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState) override;
	std::shared_ptr<IDepthStencilState> GetDepthStencilState() const override;

	void SetRenderTarget(IRenderTarget* renderTarget) override;
	IRenderTarget* GetRenderTarget() const override;

protected:
	ShaderMap                                       m_Shaders;
	TextureMap										m_Textures;
	SamplersMap										m_Samplers;

	std::shared_ptr<IBlendState>                    m_BlendState;
	std::shared_ptr<IRasterizerState>				m_RasterizerState;
	std::shared_ptr<IDepthStencilState>				m_DepthStencilState;
	IRenderTarget*                  m_RenderTarget;
};