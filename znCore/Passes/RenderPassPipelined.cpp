#include "stdafx.h"

// General
#include "RenderPassPipelined.h"

IBlendState::BlendMode RenderPassPipelined::alphaBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
IBlendState::BlendMode RenderPassPipelined::additiveBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::One, IBlendState::BlendFactor::One);
IBlendState::BlendMode RenderPassPipelined::disableBlending = IBlendState::BlendMode(false);
IDepthStencilState::DepthMode RenderPassPipelined::enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);
IDepthStencilState::DepthMode RenderPassPipelined::disableDepthWrites = IDepthStencilState::DepthMode(false, IDepthStencilState::DepthWrite::Disable);


RenderPassPipelined::RenderPassPipelined(IRenderDevice& RenderDevice)
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice.GetBaseManager())
{
	m_PerFrameConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerFrame());
}

RenderPassPipelined::~RenderPassPipelined()
{
}



std::shared_ptr<IRenderPassPipelined> RenderPassPipelined::shared_from_this()
{
	return std::dynamic_pointer_cast<IRenderPassPipelined>(IVisitor::shared_from_this());
}

std::weak_ptr<IRenderPassPipelined> RenderPassPipelined::weak_from_this()
{
	return std::weak_ptr<IRenderPassPipelined>(shared_from_this());
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
	{
		const auto& shader = shaderIt.second;
		_ASSERT(shader != nullptr);

		if (shader->GetType() == EShaderType::VertexShader)
		{
			BindPerFrameDataToVertexShader(shader.get());
		}
	}
}

void RenderPassPipelined::PostRender(RenderEventArgs& e)
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->UnBind();
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> RenderPassPipelined::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	return shared_from_this();
}

std::shared_ptr<IRenderPassPipelined> RenderPassPipelined::SetPipeline(std::shared_ptr<IPipelineState> Pipeline)
{
	_ASSERT(Pipeline);

	if (m_Pipeline != nullptr)
	{
		Log::Warn("RenderPassPipelined::SetPipeline: Pipeline already exists. Are you sure to do this ?");
	}

	m_Pipeline = Pipeline;

	return shared_from_this();
}

IPipelineState& RenderPassPipelined::GetPipeline() const
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	return *m_Pipeline;
}


void RenderPassPipelined::UpdateViewport(const Viewport& _viewport)
{
	_ASSERT_EXPR(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->GetRasterizerState()->SetViewport(&_viewport);
}



//
// IVisitor
//
bool RenderPassPipelined::Visit(const ISceneNode3D* node)
{
	return false;
}

bool RenderPassPipelined::Visit(const ISceneNodeUI* node)
{
	return false;
}

bool RenderPassPipelined::Visit(const IModel* Model)
{
    return false;
}

bool RenderPassPipelined::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return false;
}

bool RenderPassPipelined::Visit(const ISceneNodeComponent * Component)
{
	return false;
}

bool RenderPassPipelined::Visit(const ILightComponent3D* light)
{
	return false;
}



//
// Protected
//
void RenderPassPipelined::FillPerFrameData()
{
	_ASSERT(m_RenderEventArgs->Camera != nullptr);

	PerFrame perFrame;
	perFrame.View = m_RenderEventArgs->Camera->GetViewMatrix();
	perFrame.Projection = m_RenderEventArgs->Camera->GetProjectionMatrix();
	m_PerFrameConstantBuffer->Set(perFrame);
}

const RenderEventArgs& RenderPassPipelined::GetRenderEventArgs() const
{
	_ASSERT(m_RenderEventArgs != nullptr);
	return *m_RenderEventArgs;
}

const IRenderDevice& RenderPassPipelined::GetRenderDevice() const
{
    return m_RenderDevice;
}

const IBaseManager& RenderPassPipelined::GetBaseManager() const
{
	return m_BaseManager;
}



//
// Protected
//
void RenderPassPipelined::SetPerFrameData(const PerFrame& PerFrame)
{
	m_PerFrameConstantBuffer->Set(PerFrame);
}

void RenderPassPipelined::BindPerFrameDataToVertexShader(const IShader * VertexShader) const
{
	auto& perFrameParam = VertexShader->GetShaderParameterByName("PerFrame");
	if (perFrameParam.IsValid() && m_PerFrameConstantBuffer != nullptr)
	{
		perFrameParam.SetConstantBuffer(m_PerFrameConstantBuffer);
		perFrameParam.Bind();
	}
}
