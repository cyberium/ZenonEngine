#include "stdafx.h"

// General
#include "SceneCreateTypedListsPass.h"

CSceneCreateTypedListsPass::CSceneCreateTypedListsPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene)
	: ScenePass(RenderDevice, Scene)
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
bool CSceneCreateTypedListsPass::HasNodesList(SceneNodeType SceneNodeType) const
{
	return m_NodesList.find(SceneNodeType) != m_NodesList.end();
}

const std::vector<CSceneCreateTypelessListPass::SNodeElement>& CSceneCreateTypedListsPass::GetNodesList(SceneNodeType SceneNodeType) const
{
	return m_NodesList.at(SceneNodeType);
}

bool CSceneCreateTypedListsPass::HasModelsList(SceneNodeType SceneNodeType) const
{
	return m_ModelsList.find(SceneNodeType) != m_ModelsList.end();
}

const std::vector<CSceneCreateTypelessListPass::SModelElement>& CSceneCreateTypedListsPass::GetModelsList(SceneNodeType SceneNodeType) const
{
	return m_ModelsList.at(SceneNodeType);
}

bool CSceneCreateTypedListsPass::HasGeometriesList(SceneNodeType SceneNodeType) const
{
	return m_GeometryList.find(SceneNodeType) != m_GeometryList.end();
}

const std::vector<CSceneCreateTypelessListPass::SGeometryElement>& CSceneCreateTypedListsPass::GetGeometriesList(SceneNodeType SceneNodeType) const
{
	return m_GeometryList.at(SceneNodeType);
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
bool CSceneCreateTypedListsPass::Visit(const ISceneNode3D * SceneNode)
{
	if (SceneNode->GetType() < 0)
		return false;

	m_LastSceneNode = SceneNode;
	m_NodesList[SceneNode->GetType()].push_back(CSceneCreateTypelessListPass::SNodeElement(SceneNode));
	return true;
}

bool CSceneCreateTypedListsPass::Visit(const IModel * Model)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetType() >= 0);

	m_LastModel = Model;
	m_ModelsList[m_LastSceneNode->GetType()].push_back(CSceneCreateTypelessListPass::SModelElement(m_LastSceneNode, Model));
	return true;
}

bool CSceneCreateTypedListsPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetType() >= 0);
	_ASSERT(m_LastModel != nullptr);

	m_GeometryList[m_LastSceneNode->GetType()].push_back(CSceneCreateTypelessListPass::SGeometryElement(m_LastSceneNode, m_LastModel, Geometry, Material, GeometryDrawArgs));
	return true;
}

bool CSceneCreateTypedListsPass::Visit(const ILightComponent3D * light)
{
	_ASSERT(m_LastSceneNode != nullptr);
	_ASSERT(m_LastSceneNode->GetType() >= 0);

	m_LastLight = light;
	m_LightList[m_LastSceneNode->GetType()].push_back(CSceneCreateTypelessListPass::SLightElement(m_LastSceneNode, light));
	return true;
}
