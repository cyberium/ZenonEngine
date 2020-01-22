#include "stdafx.h"

// General
#include "RenderPass.h"

RenderPass::RenderPass(IRenderDevice* RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice->GetBaseManager())
{}

RenderPass::~RenderPass()
{}



//
// IRenderPass
//
void RenderPass::SetEnabled(bool Value)
{
	m_Enabled = Value;
}

bool RenderPass::IsEnabled() const
{
	return m_Enabled;
}

void RenderPass::PreRender(RenderEventArgs& e)
{
	m_RenderEventArgs = &e;
}

void RenderPass::PostRender(RenderEventArgs& e)
{}



//
// IVisitor
//
bool RenderPass::VisitBase(ISceneNode3D * node)
{
	m_RenderEventArgs->Node = node;

	const ICameraComponent3D* camera = m_RenderEventArgs->Camera;
	if (camera)
		node->UpdateCamera(camera);
	
	if (const IPipelineState* pipeline = m_RenderEventArgs->PipelineState)
		if (const Viewport* viewport = pipeline->GetRasterizerState()->GetViewports()[0])
			node->UpdateViewport(viewport);

	return true;
}

bool RenderPass::Visit3D(ISceneNode3D* node)
{
	m_RenderEventArgs->Node = node;

	return true;
}

bool RenderPass::VisitUI(ISceneNode3D* node)
{
	m_RenderEventArgs->Node = node;

	return true;
}

bool RenderPass::Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams)
{
    return true;
}

bool RenderPass::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	return true;
}

bool RenderPass::Visit(ILightComponent3D* light)
{
	return true;
}



//
// Protected
//
const RenderEventArgs* RenderPass::GetRenderEventArgs() const
{
    _ASSERT(m_RenderEventArgs != nullptr);
    return m_RenderEventArgs;
}

IRenderDevice* RenderPass::GetRenderDevice() const
{
    return m_RenderDevice;
}

const IBaseManager* RenderPass::GetBaseManager() const
{
	_ASSERT(m_BaseManager != nullptr);
	return m_BaseManager;
}
