#include "stdafx.h"

// General
#include "RenderPass.h"

RenderPass::RenderPass(IRenderDevice& RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice.GetBaseManager())
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
EVisitResult RenderPass::Visit(const ISceneNode* node)
{
	return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const ISceneNodeUI* node)
{
	return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const IModel* Model)
{
    return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const ISceneNodeComponent * Component)
{
	return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const ILight3D* light)
{
	return EVisitResult::Block;
}

EVisitResult RenderPass::Visit(const IParticleSystem * ParticleSystem)
{
	return EVisitResult::Block;
}



//
// Protected
//
const RenderEventArgs* RenderPass::GetRenderEventArgs() const
{
    _ASSERT(m_RenderEventArgs != nullptr);
    return m_RenderEventArgs;
}

IRenderDevice& RenderPass::GetRenderDevice() const
{
    return m_RenderDevice;
}

const IBaseManager& RenderPass::GetBaseManager() const
{
	return m_BaseManager;
}
