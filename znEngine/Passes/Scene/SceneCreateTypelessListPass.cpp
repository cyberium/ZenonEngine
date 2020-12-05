#include "stdafx.h"

// General
#include "SceneCreateTypelessListPass.h"


CSceneCreateTypelessListPass::CSceneCreateTypelessListPass(IRenderDevice& RenderDevice, IScene& Scene)
	: ScenePass(Scene)
{}

CSceneCreateTypelessListPass::~CSceneCreateTypelessListPass()
{}

//
// CSceneCreateTypelessListPass
//
const std::vector<IRenderPassCreateTypelessList::SNodeElement>& CSceneCreateTypelessListPass::GetNodesList() const
{
	return m_NodesList;
}

const std::vector<IRenderPassCreateTypelessList::SModelElement>& CSceneCreateTypelessListPass::GetModelsList() const
{
	return m_ModelsList;
}

const std::vector<IRenderPassCreateTypelessList::SGeometryElement>& CSceneCreateTypelessListPass::GetGeometryList() const
{
	return m_GeometryList;
}

const std::vector<IRenderPassCreateTypelessList::SLightElement>& CSceneCreateTypelessListPass::GetLightList() const
{
	return m_LightList;
}

//
// IRenderPass
//
void CSceneCreateTypelessListPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);
	m_NodesList.clear();
	m_ModelsList.clear();
	m_GeometryList.clear();
	m_LightList.clear();
}

void CSceneCreateTypelessListPass::Render(RenderEventArgs & e)
{
	ScenePass::Render(e);
}

//
// IVisitor
//
EVisitResult CSceneCreateTypelessListPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	// TODO Here?
	if (auto colliderComponent = SceneNode->GetComponentT<IColliderComponent>())
		if (colliderComponent->IsCulled(GetRenderEventArgs().CameraForCulling))
			return EVisitResult::Block;

	m_NodesList.push_back(SNodeElement(SceneNode));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypelessListPass::Visit(const std::shared_ptr<IModel>& Model)
{
	_ASSERT(! m_NodesList.empty());
	const auto& lastNodeElement = m_NodesList.back();
	//lastNodeElement.ModelsElements.push_back(SModelElement(lastNodeElement.SceneNode, Model));
	m_ModelsList.push_back(SModelElement(lastNodeElement.SceneNode, Model));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypelessListPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	_ASSERT(! m_ModelsList.empty());
	const auto& lastModelElement = m_ModelsList.back();
	//lastModelElement.GeometriesElements.push_back(SGeometryElement(lastModelElement.SceneNode, lastModelElement.Model, Geometry, Material, GeometryDrawArgs));
	m_GeometryList.push_back(SGeometryElement(lastModelElement.SceneNode, lastModelElement.Model, Geometry, Material, GeometryDrawArgs));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypelessListPass::Visit(const std::shared_ptr<ILight3D>& light)
{
	_ASSERT(! m_NodesList.empty());
	const auto& lastNodeElement = m_NodesList.back();
	//lastNodeElement.LightsElement.push_back(SLightElement(lastNodeElement.SceneNode, light));
	m_LightList.push_back(SLightElement(lastNodeElement.SceneNode, light));
	return EVisitResult::AllowAll;
}
