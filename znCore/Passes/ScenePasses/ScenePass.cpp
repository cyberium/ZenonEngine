#include "stdafx.h"

// General
#include "ScenePass.h"

ScenePass::ScenePass(IScene& Scene)
	: RenderPass(Scene.GetRenderDevice())
	, m_Scene(Scene)
{}

ScenePass::~ScenePass()
{}

//
// IRenderPass
//
void ScenePass::Render(RenderEventArgs & e)
{
	GetScene().Accept(this);
}



//
// IVisitor
//
EVisitResult ScenePass::Visit(const ISceneNode* node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const IUIControl* node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const IModel* Model)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const ISceneNodeComponent * Component)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const ILight3D* light)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const IParticleSystem * ParticleSystem)
{
	return EVisitResult::Block;
}



//
// Protected
//
IScene& ScenePass::GetScene() const
{
	return m_Scene;
}
