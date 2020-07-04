#include "stdafx.h"

// General
#include "SceneNodesSelector.h"

void CSceneNodesSelector::Selector_Block()
{
	m_SelectionBlocked = true;
}

void CSceneNodesSelector::Selector_Unblock()
{
	m_SelectionBlocked = false;
}

void CSceneNodesSelector::Selector_SelectNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync)
{
	m_SelectedNodes.clear();
	m_SelectedNodes.push_back(Node);

	if (NeedSync)
		Selector_SyncronizeWithOtherSelector();
	Selector_OnSelectionChange();
}

void CSceneNodesSelector::Selector_SelectNodes(const SelectedNodes & Nodes, bool NeedSync)
{
	m_SelectedNodes = Nodes;

	if (NeedSync)
		Selector_SyncronizeWithOtherSelector();
	Selector_OnSelectionChange();
}

void CSceneNodesSelector::Selector_ClearSelection(bool NeedSync)
{
	m_SelectedNodes.clear();

	if (NeedSync)
		Selector_SyncronizeWithOtherSelector();
	Selector_OnSelectionChange();
}

void CSceneNodesSelector::Selector_AddNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync)
{
	if (std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node) != m_SelectedNodes.end())
		return;
	m_SelectedNodes.push_back(Node);

	if (NeedSync)
		Selector_SyncronizeWithOtherSelector();
	Selector_OnSelectionChange();
}

void CSceneNodesSelector::Selector_RemoveNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync)
{
	auto it = std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node);
	if (it == m_SelectedNodes.end())
		return;
	m_SelectedNodes.erase(it);

	if (NeedSync)
		Selector_SyncronizeWithOtherSelector();
	Selector_OnSelectionChange();
}

std::shared_ptr<ISceneNode3D> CSceneNodesSelector::Selector_GetFirstSelectedNode()
{
	auto first = m_SelectedNodes.begin();
	if (first == m_SelectedNodes.end())
		return nullptr;

	return *first;
}

const CSceneNodesSelector::SelectedNodes& CSceneNodesSelector::Selector_GetSelectedNodes()
{
	return m_SelectedNodes;
}

void CSceneNodesSelector::Selector_SetOtherSelector(CSceneNodesSelector * OtherSelector)
{
	m_OtherSelector = OtherSelector;
}

void CSceneNodesSelector::Selector_SyncronizeWithOtherSelector()
{
	if (m_OtherSelector)
	{
		m_OtherSelector->Selector_SelectNodes(Selector_GetSelectedNodes(), false);
	}
	else
		_ASSERT(false);
}

void CSceneNodesSelector::Selector_OnSelectionChange()
{

}
