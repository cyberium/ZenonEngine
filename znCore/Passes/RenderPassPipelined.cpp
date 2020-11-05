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
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice.GetBaseManager())
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
void RenderPassPipelined::SetEnabled(bool Value)
{
	m_Enabled = Value;
}

bool RenderPassPipelined::IsEnabled() const
{
	return m_Enabled;
}

void RenderPassPipelined::PreRender(RenderEventArgs& e)
{
	m_RenderEventArgs = &e;

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
// IVisitor
//
EVisitResult RenderPassPipelined::Visit(const ISceneNode* node)
{
	return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const IUIControl* node)
{
	return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const IModel* Model)
{
    return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const ISceneNodeComponent * Component)
{
	return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const ILight3D* light)
{
	return EVisitResult::Block;
}

EVisitResult RenderPassPipelined::Visit(const IParticleSystem * ParticleSystem)
{
	return EVisitResult::Block;
}



//
// Protected
//
IBaseManager& RenderPassPipelined::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& RenderPassPipelined::GetRenderDevice() const
{
	return m_RenderDevice;
}

void RenderPassPipelined::FillPerFrameData()
{
	_ASSERT(m_RenderEventArgs->Camera != nullptr);

	PerFrame perFrame(
		m_RenderEventArgs->Camera->GetViewMatrix(), 
		m_RenderEventArgs->Camera->GetProjectionMatrix(), 
		m_RenderEventArgs->PipelineState->GetRenderTarget()->GetViewport().GetSize()
	);
	m_PerFrameConstantBuffer->Set(perFrame);
}

const RenderEventArgs& RenderPassPipelined::GetRenderEventArgs() const
{
	_ASSERT(m_RenderEventArgs != nullptr);
	return *m_RenderEventArgs;
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
