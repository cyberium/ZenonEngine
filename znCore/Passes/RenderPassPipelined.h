#pragma once

#include "RenderPass.h"

class ZN_API RenderPassPipelined
	: public RenderPass
	, public IRenderPassPipelined
	, public std::enable_shared_from_this<IRenderPassPipelined>
{
public:
	RenderPassPipelined(IRenderDevice& RenderDevice);
	virtual ~RenderPassPipelined();

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e);
	virtual void PostRender(RenderEventArgs& e);
    
	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	virtual IPipelineState& GetPipeline() const override;

protected:
	virtual PerFrame GetPerFrameData() const;
	void BindPerFrameData() const;

private:
	std::shared_ptr<IPipelineState> m_Pipeline;
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
};