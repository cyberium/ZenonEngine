#pragma once

#include "RenderPass.h"

class ZN_API RenderPassMultipipelined
	: public RenderPass
	, public IRenderPassMultipipelinded
	, public std::enable_shared_from_this<IRenderPassPipelined>
{
public:
	RenderPassMultipipelined(IRenderDevice& RenderDevice);
	virtual ~RenderPassMultipipelined();

	virtual void PreRender(IPipelineState& Pipeline, RenderEventArgs& e);
	virtual void Render(IPipelineState& Pipeline, RenderEventArgs& e);
	virtual void PostRender(IPipelineState& Pipeline, RenderEventArgs& e);

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;
    
	// IRenderPassPipelined
	void Configure(std::shared_ptr<IRenderTarget> OutputRenderTarget) override;
	const std::vector<std::shared_ptr<IPipelineState>>& GetPipelines() const override;

protected:
	virtual PerFrame GetPerFrameData(IPipelineState& Pipeline) const;
	void BindPerFrameData(IPipelineState& Pipeline) const;

private:
	std::vector<std::shared_ptr<IPipelineState>> m_Pipelines;
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
};