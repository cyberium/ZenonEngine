#include "stdafx.h"

// General
#include "RenderPassPipelined.h"

IBlendState::BlendMode RenderPassPipelined::alphaBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
IBlendState::BlendMode RenderPassPipelined::additiveBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::One, IBlendState::BlendFactor::One);
IBlendState::BlendMode RenderPassPipelined::disableBlending = IBlendState::BlendMode(false);
IDepthStencilState::DepthMode RenderPassPipelined::enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);
IDepthStencilState::DepthMode RenderPassPipelined::enableTestDisableWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Disable);
IDepthStencilState::DepthMode RenderPassPipelined::disableDepthWrites = IDepthStencilState::DepthMode(false, IDepthStencilState::DepthWrite::Disable);


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

	FillPerFrameData();

	for (const auto& shaderIt : m_Pipeline->GetShaders())
		BindPerFrameData(shaderIt.second.get());
}

void RenderPassPipelined::PostRender(RenderEventArgs& e)
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->UnBind();

	__super::PostRender(e);
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> RenderPassPipelined::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
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
void RenderPassPipelined::FillPerFrameData()
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
	m_PerFrameConstantBuffer->Set(perFrame);
}



//
// Protected
//
void RenderPassPipelined::SetPerFrameData(const PerFrame& PerFrame)
{
	m_PerFrameConstantBuffer->Set(PerFrame);
}

void RenderPassPipelined::BindPerFrameData(const IShader * Shader) const
{
	_ASSERT(Shader != nullptr);

	auto& perFrameParam = Shader->GetShaderParameterByName("PerFrame");
	if (perFrameParam.IsValid() && m_PerFrameConstantBuffer != nullptr)
	{
		perFrameParam.SetConstantBuffer(m_PerFrameConstantBuffer);
		perFrameParam.Bind();
	}
}
