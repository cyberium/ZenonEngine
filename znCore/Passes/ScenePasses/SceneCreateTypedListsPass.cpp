#include "stdafx.h"

// General
#include "SceneCreateTypedListsPass.h"

CSceneCreateTypedListsPass::CSceneCreateTypedListsPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene)
	: ScenePass(RenderDevice, Scene)
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
	return m_NodesByType.find(SceneNodeType) != m_NodesByType.end();
}

const std::vector<CSceneCreateTypelessListPass::SNodeElement>& CSceneCreateTypedListsPass::GetNodesList(SceneNodeType SceneNodeType) const
{
	if (m_NodesByType.find(SceneNodeType) == m_NodesByType.end())
		return m_EmptyList;
	return m_NodesByType.at(SceneNodeType);
}

//
// IRenderPass
//
void CSceneCreateTypedListsPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);
	m_NodesByType.clear();
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
	SceneNodeType type = SceneNode->GetType();
	if (SceneNode->GetType() < 0)
		return false;

	m_NodesByType[type].push_back(CSceneCreateTypelessListPass::SNodeElement(SceneNode));
	return false; // To prevent visit into components
}