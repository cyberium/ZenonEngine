#pragma once

class ZN_API CPassPostprocess_HDR
	: public RenderPassPipelined
{
public:
	CPassPostprocess_HDR(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> HDRRenderTarget);
	virtual ~CPassPostprocess_HDR();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

private:
	std::shared_ptr<IRenderTarget> m_HDRRenderTarget;
	std::shared_ptr<IGeometry> m_QuadGeometry;
};