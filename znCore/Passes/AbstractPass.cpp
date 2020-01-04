#include "stdafx.h"

// General
#include "AbstractPass.h"

AbstractPass::AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(nullptr)
    , m_RenderDevice(RenderDevice)
{}

AbstractPass::AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IPipelineState> Pipeline)
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(Pipeline)
    , m_RenderDevice(RenderDevice)
{}

AbstractPass::~AbstractPass()
{}

void AbstractPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool AbstractPass::IsEnabled() const
{
	return m_Enabled;
}

void AbstractPass::PreRender(RenderEventArgs& e)
{
    SetRenderEventArgs(&e);

    std::shared_ptr<IPipelineState> pipelineState = GetPipelineState();
    if (pipelineState)
    {
        e.PipelineState = pipelineState.get();
        pipelineState->Bind();
    }
}

void AbstractPass::PostRender(RenderEventArgs& e)
{
    std::shared_ptr<IPipelineState> pipelineState = GetPipelineState();
    if (pipelineState)
    {
        pipelineState->UnBind();
    }
}


void AbstractPass::UpdateViewport(const Viewport * _viewport)
{
    std::shared_ptr<IPipelineState> pipelineState = GetPipelineState();
    if (pipelineState)
    {
        pipelineState->GetRasterizerState()->SetViewport(_viewport);
    }
}



//
// IVisitor
//
bool AbstractPass::VisitBase(ISceneNode * node)
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

bool AbstractPass::Visit3D(ISceneNode* node)
{
	GetRenderEventArgs()->Node = node;

	return false;
}

bool AbstractPass::VisitUI(ISceneNode* node)
{
	GetRenderEventArgs()->Node = node;

	return false;
}

bool AbstractPass::Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams)
{
	_ASSERT(false, "This function must be specialized.");
    return false;
}

bool AbstractPass::Visit(IGeometry * Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams)
{
	_ASSERT(false, "This function must be specialized.");
	return false;
}

bool AbstractPass::Visit(std::shared_ptr<CLight3D> light)
{
    _ASSERT(false);
	return false;
}

void AbstractPass::SetRenderEventArgs(RenderEventArgs* e)
{
    m_RenderEventArgs = e;
}

RenderEventArgs* AbstractPass::GetRenderEventArgs() const
{
    assert(m_RenderEventArgs);
    return m_RenderEventArgs;
}

std::shared_ptr<IPipelineState> AbstractPass::GetPipelineState() const
{
    return m_Pipeline;
}

std::shared_ptr<IRenderDevice> AbstractPass::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}
