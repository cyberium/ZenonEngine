#include "stdafx.h"

// General
#include "RenderPassPipelined.h"

IBlendState::BlendMode RenderPassPipelined::alphaBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
IBlendState::BlendMode RenderPassPipelined::disableBlending = IBlendState::BlendMode(false);
IDepthStencilState::DepthMode RenderPassPipelined::enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);
IDepthStencilState::DepthMode RenderPassPipelined::disableDepthWrites = IDepthStencilState::DepthMode(false, IDepthStencilState::DepthWrite::Disable);


RenderPassPipelined::RenderPassPipelined(std::shared_ptr<IRenderDevice> RenderDevice)
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice->GetBaseManager())
{
	m_PerFrameData = (PerFrame*)_aligned_malloc(sizeof(PerFrame), 16);
	m_PerFrameConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerFrame());
}

RenderPassPipelined::~RenderPassPipelined()
{
	_aligned_free(m_PerFrameData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerFrameConstantBuffer);
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

	_ASSERT(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
    e.PipelineState = m_Pipeline.get();
	m_Pipeline->Bind();

	FillPerFrameData();

	for (const auto& shaderIt : m_Pipeline->GetShaders())
	{
		const IShader* shader = shaderIt.second.get();
		_ASSERT(shader != nullptr);

		if (shader->GetType() == EShaderType::VertexShader)
		{
			IShaderParameter* perObjectParameter = shader->GetShaderParameterByName("PerFrame").get();
			if (perObjectParameter->IsValid() && m_PerFrameConstantBuffer != nullptr)
			{
				perObjectParameter->SetConstantBuffer(m_PerFrameConstantBuffer.get());
				perObjectParameter->Bind();
			}
		}
	}
}

void RenderPassPipelined::PostRender(RenderEventArgs& e)
{
	_ASSERT(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->UnBind();
}



//
// IRenderPassPipelined
//
void RenderPassPipelined::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
}

void RenderPassPipelined::SetPipeline(std::shared_ptr<IPipelineState> Pipeline)
{
	_ASSERT(Pipeline);

	if (m_Pipeline != nullptr)
	{
		Log::Warn("RenderPassPipelined::SetPipeline: Pipeline already exists. Are you sure to do this ?");
	}

	m_Pipeline = Pipeline;
}

std::shared_ptr<IPipelineState> RenderPassPipelined::GetPipeline() const
{
	_ASSERT(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	return m_Pipeline;
}


void RenderPassPipelined::UpdateViewport(const Viewport * _viewport)
{
	_ASSERT(m_Pipeline != nullptr, L"RenderPassPipelined: Pipeline is null. Don't use this class without pipeline.");
	m_Pipeline->GetRasterizerState()->SetViewport(_viewport);
}



//
// IVisitor
//
bool RenderPassPipelined::VisitBase(ISceneNode * node)
{
	GetRenderEventArgs()->Node = node;

	const ICamera* camera = GetRenderEventArgs()->Camera;
	if (camera)
		node->UpdateCamera(camera);
	
	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	if (viewport)
		node->UpdateViewport(viewport);

	return true;
}

bool RenderPassPipelined::Visit3D(ISceneNode* node)
{
	GetRenderEventArgs()->Node = node;

	return true;
}

bool RenderPassPipelined::VisitUI(ISceneNode* node)
{
	GetRenderEventArgs()->Node = node;

	return true;
}

bool RenderPassPipelined::Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams)
{
    return true;
}

bool RenderPassPipelined::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	return true;
}

bool RenderPassPipelined::Visit(ILightComponent3D* light)
{
	return true;
}



//
// Protected
//
void RenderPassPipelined::FillPerFrameData()
{
	_ASSERT(m_RenderEventArgs->Camera != nullptr);
	m_PerFrameData->View = m_RenderEventArgs->Camera->GetViewMatrix();
	m_PerFrameData->Projection = m_RenderEventArgs->Camera->GetProjectionMatrix();
	m_PerFrameConstantBuffer->Set(*m_PerFrameData);
}

RenderEventArgs* RenderPassPipelined::GetRenderEventArgs() const
{
	_ASSERT(m_RenderEventArgs != nullptr);
	return m_RenderEventArgs;
}

std::shared_ptr<IRenderDevice> RenderPassPipelined::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}

const IBaseManager* RenderPassPipelined::GetBaseManager() const
{
	_ASSERT(m_BaseManager != nullptr);
	return m_BaseManager;
}
