#include <stdafx.h>

// General
#include "AbstractPass.h"

AbstractPass::AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(nullptr)
    , m_RenderDevice(RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject());
}

AbstractPass::AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IPipelineState> Pipeline)
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(Pipeline)
    , m_RenderDevice(RenderDevice)
{
    m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
    m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject());
}

AbstractPass::~AbstractPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

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
	return false;
}

bool AbstractPass::Visit3D(ISceneNode* node)
{
	return false;
}

bool AbstractPass::VisitUI(ISceneNode* nodeUI)
{
	return false;
}

bool AbstractPass::Visit(IMesh* Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
    if (m_RenderEventArgs)
    {
        return Mesh->Render(m_RenderEventArgs, m_PerObjectConstantBuffer.get(), IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
    }

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

void AbstractPass::SetPerObjectConstantBufferData(PerObject& perObjectData)
{
	m_PerObjectConstantBuffer->Set(perObjectData);
}

std::shared_ptr<IConstantBuffer> AbstractPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void AbstractPass::BindPerObjectConstantBuffer(std::shared_ptr<IShader> shader)
{
    if (shader)
        shader->GetShaderParameterByName("PerObject")->Set(m_PerObjectConstantBuffer.get());
}

std::shared_ptr<IPipelineState> AbstractPass::GetPipelineState() const
{
    return m_Pipeline;
}

std::shared_ptr<IRenderDevice> AbstractPass::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}
