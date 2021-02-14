#pragma once

#include "RenderPass.h"

class ZN_API RenderPassMultipipelined
	: public RenderPass
	, public IRenderPassMultipipelinded
{
public:
	RenderPassMultipipelined(IRenderDevice& RenderDevice);
	virtual ~RenderPassMultipipelined();

	virtual void PreRender(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs& e);
	virtual void Render(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs& e);
	virtual void PostRender(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs& e);

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;
    
	// IRenderPassMultipipelinded
	void Configure(std::shared_ptr<IRenderTarget> OutputRenderTarget) override;
	const std::vector<std::shared_ptr<IPipelineState>>& GetPipelines() const override;

protected:
	virtual PerFrame GetPerFrameData(IPipelineState& Pipeline) const;
	void BindPerFrameData(IPipelineState& Pipeline) const;

	size_t AddPipeline(std::shared_ptr<IPipelineState> Pipeline);

private:
	std::vector<std::shared_ptr<IPipelineState>> m_Pipelines;
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
};