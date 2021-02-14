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

void RenderPassMultipipelined::PreRender(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e)
{
	e.PipelineState = &Pipeline;
	Pipeline.Bind();

	BindPerFrameData(Pipeline);
}

void RenderPassMultipipelined::Render(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e)
{
}

void RenderPassMultipipelined::PostRender(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e)
{
	Pipeline.UnBind();
	e.PipelineState = nullptr;
}



//
// IRenderPass
//
void RenderPassMultipipelined::Render(RenderEventArgs & e)
{
	const auto& pipelines = GetPipelines();
	for (size_t i = 0; i < pipelines.size(); i++)
	{
		const auto& pipeline = pipelines.at(i);
		PreRender(i, *pipeline, e);
		Render(i, *pipeline, e);
		PostRender(i, *pipeline, e);
	}
}



//
// IRenderPassMultipipelinded
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
		Pipeline.GetRenderTarget()->GetViewport().GetSize()
	);
	return perFrame;
}

void RenderPassMultipipelined::BindPerFrameData(IPipelineState& Pipeline) const
{
	m_PerFrameConstantBuffer->Set(GetPerFrameData(Pipeline));

	for (const auto& shaderIt : Pipeline.GetShaders())
	{
		auto* perFrameParam = shaderIt.second->GetShaderParameterByName("PerFrame");
		if (perFrameParam)
		{
			perFrameParam->SetConstantBuffer(m_PerFrameConstantBuffer);
			perFrameParam->Bind();
		}
	}
}

size_t RenderPassMultipipelined::AddPipeline(std::shared_ptr<IPipelineState> Pipeline)
{
	m_Pipelines.push_back(Pipeline);
	return m_Pipelines.size() - 1;
}
