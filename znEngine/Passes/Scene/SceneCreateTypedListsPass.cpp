#include "stdafx.h"

// General
#include "SceneCreateTypedListsPass.h"

CSceneCreateTypedListsPass::CSceneCreateTypedListsPass(IRenderDevice& RenderDevice, IScene& Scene)
	: ScenePass(Scene)
	, m_LastSceneNode(nullptr)
	, m_LastModel(nullptr)
	, m_LastGeometry(nullptr)
	, m_LastLight(nullptr)
{
}

CSceneCreateTypedListsPass::~CSceneCreateTypedListsPass()
{
}

//
// CSceneCreateTypedListsPass
//
bool CSceneCreateTypedListsPass::HasNodesList(ObjectClass SceneNodeType) const
{
	return m_NodesList.find(SceneNodeType) != m_NodesList.end();
}

const std::vector<IRenderPassCreateTypelessList::SNodeElement>& CSceneCreateTypedListsPass::GetNodesList(ObjectClass SceneNodeType) const
{
	return m_NodesList.at(SceneNodeType);
}

bool CSceneCreateTypedListsPass::HasModelsList(ObjectClass SceneNodeType) const
{
	return m_ModelsList.find(SceneNodeType) != m_ModelsList.end();
}

const std::vector<IRenderPassCreateTypelessList::SModelElement>& CSceneCreateTypedListsPass::GetModelsList(ObjectClass SceneNodeType) const
{
	return m_ModelsList.at(SceneNodeType);
}

bool CSceneCreateTypedListsPass::HasGeometriesList(ObjectClass SceneNodeType) const
{
	return m_GeometryList.find(SceneNodeType) != m_GeometryList.end();
}

const std::vector<IRenderPassCreateTypelessList::SGeometryElement>& CSceneCreateTypedListsPass::GetGeometriesList(ObjectClass SceneNodeType) const
{
	return m_GeometryList.at(SceneNodeType);
}

bool CSceneCreateTypedListsPass::HasParticleSystemsList(ObjectClass SceneNodeType) const
{
	return m_ParticleSystemList.find(SceneNodeType) != m_ParticleSystemList.end();
}

const std::vector<IRenderPassCreateTypelessList::SParticleSystemElement>& CSceneCreateTypedListsPass::GetParticleSystemList(ObjectClass SceneNodeType) const
{
	return m_ParticleSystemList.at(SceneNodeType);
}

//
// IRenderPass
//
void CSceneCreateTypedListsPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);
	m_NodesList.clear();
	m_ModelsList.clear();
	m_GeometryList.clear();
	m_LightList.clear();
}

void CSceneCreateTypedListsPass::Render(RenderEventArgs & e)
{
	ScenePass::Render(e);
}

//
// IVisitor
//
EVisitResult CSceneCreateTypedListsPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	_ASSERT(SceneNode != nullptr);

	ObjectClass sceneNodeClass = SceneNode->GetClass();
	if (sceneNodeClass <= 0)
		return EVisitResult::AllowVisitChilds;

	// TODO Here?
	if (auto colliderComponent = SceneNode->GetComponentT<IColliderComponent>())
		if (colliderComponent->IsCulled(GetRenderEventArgs().CameraForCulling))
			return EVisitResult::Block;

	m_LastSceneNode = SceneNode;
	m_NodesList[sceneNodeClass].push_back(CSceneCreateTypelessListPass::SNodeElement(SceneNode));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypedListsPass::Visit(const std::shared_ptr<IModel>& Model)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetClass() >= 0);

	m_LastModel = Model;
	m_ModelsList[m_LastSceneNode->GetClass()].push_back(CSceneCreateTypelessListPass::SModelElement(m_LastSceneNode, Model));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypedListsPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetClass() > 0);
	_ASSERT(m_LastModel != nullptr);

	m_GeometryList[m_LastSceneNode->GetClass()].push_back(CSceneCreateTypelessListPass::SGeometryElement(m_LastSceneNode, m_LastModel, Geometry, Material, GeometryDrawArgs));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypedListsPass::Visit(const std::shared_ptr<ILight>& light)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetClass() > 0);

	m_LastLight = light;
	m_LightList[m_LastSceneNode->GetClass()].push_back(CSceneCreateTypelessListPass::SLightElement(m_LastSceneNode, light));
	return EVisitResult::AllowAll;
}

EVisitResult CSceneCreateTypedListsPass::Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetClass() > 0);

	m_LastParticleSystem = ParticleSystem;
	m_ParticleSystemList[m_LastSceneNode->GetClass()].push_back(CSceneCreateTypelessListPass::SParticleSystemElement(m_LastSceneNode, ParticleSystem));
	return EVisitResult::AllowAll;
}
