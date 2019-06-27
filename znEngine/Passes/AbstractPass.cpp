#include <stdafx.h>

// General
#include "AbstractPass.h"

// Additional
#include "Application.h"

AbstractPass::AbstractPass()
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(nullptr)
    , m_RenderDevice(_RenderDevice)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

AbstractPass::AbstractPass(std::shared_ptr<PipelineState> Pipeline)
    : m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_Pipeline(Pipeline)
    , m_RenderDevice(_RenderDevice)
{
    m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
    m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

AbstractPass::~AbstractPass()
{
	_aligned_free(m_PerObjectData);
	_RenderDevice->DestroyConstantBuffer(m_PerObjectConstantBuffer);
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
    std::shared_ptr<PipelineState> pipelineState = GetPipelineState();
    _ASSERT(pipelineState);

    e.PipelineState = pipelineState.get();
    SetRenderEventArgs(&e);

    if (pipelineState)
    {
        pipelineState->Bind();
    }
}

void AbstractPass::PostRender(RenderEventArgs& e)
{
    std::shared_ptr<PipelineState> pipelineState = GetPipelineState();
    _ASSERT(pipelineState);

    if (pipelineState)
    {
        pipelineState->UnBind();
    }
}


void AbstractPass::UpdateViewport(Viewport _viewport)
{
    GetPipelineState()->GetRasterizerState().SetViewport(_viewport);
}



//
// IVisitor
//
bool AbstractPass::Visit(std::shared_ptr<SceneNode3D> node)
{
    fail1();
	return false;
}

bool AbstractPass::Visit(std::shared_ptr<CUIBaseNode> nodeUI)
{
	fail1();
	return false;
}

bool AbstractPass::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
    if (m_RenderEventArgs)
    {
        return Mesh->Render(m_RenderEventArgs, m_PerObjectConstantBuffer, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
    }

    return false;
}

bool AbstractPass::Visit(std::shared_ptr<CLight3D> light)
{
    fail1();
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

std::shared_ptr<ConstantBuffer> AbstractPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void AbstractPass::BindPerObjectConstantBuffer(std::shared_ptr<Shader> shader)
{
    if (shader)
        shader->GetShaderParameterByName("PerObject").Set(m_PerObjectConstantBuffer);
}

std::shared_ptr<PipelineState> AbstractPass::GetPipelineState() const
{
    return m_Pipeline;
}

std::shared_ptr<IRenderDevice> AbstractPass::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}
