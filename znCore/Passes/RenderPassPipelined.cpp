#include "stdafx.h"

// General
#include "RenderPassPipelined.h"


RenderPassPipelined::RenderPassPipelined(IRenderDevice& RenderDevice)
    : RenderPass(RenderDevice)
{
	m_Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

RenderPassPipelined::~RenderPassPipelined()
{
}



//
// IRenderPass
//
void RenderPassPipelined::PreRender(RenderEventArgs& e)
{
	__super::PreRender(e);

	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
    e.PipelineState = m_Pipeline.get();

	m_Pipeline->Bind();

	BindPerFrameData();
}

void RenderPassPipelined::PostRender(RenderEventArgs& e)
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->UnBind();

	e.PipelineState = nullptr;
	__super::PostRender(e);
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> RenderPassPipelined::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	return shared_from_this();
}

IPipelineState& RenderPassPipelined::GetPipeline() const
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	return *m_Pipeline;
}



//
// Protected
//
PerFrame RenderPassPipelined::GetPerFrameData() const
{
	const ICameraComponent3D* camera = GetRenderEventArgs().Camera;
	_ASSERT(camera != nullptr);

	const IPipelineState* pipeline = GetRenderEventArgs().PipelineState;
	_ASSERT(pipeline != nullptr && pipeline == m_Pipeline.get());

	PerFrame perFrame(
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix(),
		pipeline->GetRenderTarget()->GetViewport().GetSize()
	);
	return perFrame;
}

void RenderPassPipelined::BindPerFrameData() const
{
	m_PerFrameConstantBuffer->Set(GetPerFrameData());

	for (const auto& shaderIt : GetPipeline().GetShaders())
	{
		auto& perFrameParam = shaderIt.second->GetShaderParameterByName("PerFrame");
		if (perFrameParam.IsValid())
		{
			perFrameParam.SetConstantBuffer(m_PerFrameConstantBuffer);
			perFrameParam.Bind();
		}
	}
}
