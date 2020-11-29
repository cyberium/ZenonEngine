#include "stdafx.h"

// General
#include "RenderPassMultipipelined.h"


RenderPassMultipipelined::RenderPassMultipipelined(IRenderDevice& RenderDevice)
    : RenderPass(RenderDevice)
{
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

RenderPassMultipipelined::~RenderPassMultipipelined()
{
}

void RenderPassMultipipelined::PreRender(IPipelineState& Pipeline, RenderEventArgs & e)
{
	e.PipelineState = &Pipeline;
	Pipeline.Bind();

	BindPerFrameData(Pipeline);
}

void RenderPassMultipipelined::Render(IPipelineState& Pipeline, RenderEventArgs & e)
{
}

void RenderPassMultipipelined::PostRender(IPipelineState& Pipeline, RenderEventArgs & e)
{
	Pipeline.UnBind();
	e.PipelineState = nullptr;
}



//
// IRenderPass
//
void RenderPassMultipipelined::Render(RenderEventArgs & e)
{
	for (const auto& pipeline : GetPipelines())
	{
		PreRender(*pipeline, e);
		Render(*pipeline, e);
		PostRender(*pipeline, e);
	}
}



//
// IRenderPassPipelined
//
void RenderPassMultipipelined::Configure(std::shared_ptr<IRenderTarget> OutputRenderTarget)
{
}

const std::vector<std::shared_ptr<IPipelineState>>& RenderPassMultipipelined::GetPipelines() const
{
	return m_Pipelines;
}



//
// Protected
//
PerFrame RenderPassMultipipelined::GetPerFrameData(IPipelineState& Pipeline) const
{
	const ICameraComponent3D* camera = GetRenderEventArgs().Camera;
	_ASSERT(camera != nullptr);

	PerFrame perFrame(
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix(),
		Pipeline.GetRenderTarget()->GetViewport().GetSize(),
		camera->GetDirection()
	);
	return perFrame;
}

void RenderPassMultipipelined::BindPerFrameData(IPipelineState& Pipeline) const
{
	m_PerFrameConstantBuffer->Set(GetPerFrameData(Pipeline));

	for (const auto& shaderIt : Pipeline.GetShaders())
	{
		auto& perFrameParam = shaderIt.second->GetShaderParameterByName("PerFrame");
		if (perFrameParam.IsValid())
		{
			perFrameParam.SetConstantBuffer(m_PerFrameConstantBuffer);
			perFrameParam.Bind();
		}
	}
}
