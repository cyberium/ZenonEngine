#pragma once

class ZN_API CPassPostprocessBase
	: public RenderPassPipelined
{
public:
	CPassPostprocessBase(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture);
	virtual ~CPassPostprocessBase();

	std::shared_ptr<ITexture> GetOutputTexture() const;

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

protected:
	virtual std::shared_ptr<IRenderTarget> LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget) = 0;
	virtual std::shared_ptr<IShader> LoadVertexShader() = 0;
	virtual std::shared_ptr<IShader> LoadPixelShader() = 0;

protected:
	std::shared_ptr<IRenderTarget> CreateRenderTarget(std::shared_ptr<IRenderTarget> RenderTarget);
	void SetOutputTexture(std::shared_ptr<ITexture> Texture);

private:
	std::shared_ptr<ITexture> m_InputTexture;
	std::shared_ptr<ITexture> m_OutputTexture;
	bool m_IsCustomRenderTarget;
	std::shared_ptr<IGeometry> m_QuadGeometry;
};