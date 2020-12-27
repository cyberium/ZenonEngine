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
EVisitResult ScenePass::Visit(const std::shared_ptr<ISceneNode>& node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<IUIControl>& node)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<IModel>& Model)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<ISceneNodeComponent>& Component)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<ILight>& light)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem)
{
	return EVisitResult::Block;
}

EVisitResult ScenePass::Visit(const std::shared_ptr<ITerrain>& Terrain)
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
