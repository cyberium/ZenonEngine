#include "stdafx.h"

// General
#include "NodesSelector.h"

CSceneNodesSelector::CSceneNodesSelector(IEditor_NodesSelectorEventListener * Listener)
	: m_OtherSelector(nullptr)
	, m_SelectionEventsListener(Listener)
{
}

CSceneNodesSelector::~CSceneNodesSelector()
{
}

void CSceneNodesSelector::SelectNode(std::shared_ptr<ISceneNode3D> Node)
{
	m_SelectedNodes.clear();
	m_SelectedNodes.push_back(Node);

	DoSynchronizeWithOtherSelector();
	m_SelectionEventsListener->OnSelectNodes();
}

void CSceneNodesSelector::SelectNodes(const SelectedNodes & Nodes)
{
	m_SelectedNodes = Nodes;

	DoSynchronizeWithOtherSelector();
	m_SelectionEventsListener->OnSelectNodes();
}

void CSceneNodesSelector::ClearSelection()
{
	m_SelectedNodes.clear();

	DoSynchronizeWithOtherSelector();
	m_SelectionEventsListener->OnSelectNodes();
}

void CSceneNodesSelector::AddNode(std::shared_ptr<ISceneNode3D> Node)
{
	if (std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node) != m_SelectedNodes.end())
		return;
	m_SelectedNodes.push_back(Node);

	DoSynchronizeWithOtherSelector();
	m_SelectionEventsListener->OnSelectNodes();
}

void CSceneNodesSelector::RemoveNode(std::shared_ptr<ISceneNode3D> Node)
{
	auto it = std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node);
	if (it == m_SelectedNodes.end())
		return;
	m_SelectedNodes.erase(it);

	DoSynchronizeWithOtherSelector();
	m_SelectionEventsListener->OnSelectNodes();
}

std::shared_ptr<ISceneNode3D> CSceneNodesSelector::GetFirstSelectedNode()
{
	auto first = m_SelectedNodes.begin();
	if (first == m_SelectedNodes.end())
		return nullptr;

	return *first;
}

bool CSceneNodesSelector::IsNodeSelected(std::shared_ptr<ISceneNode3D> Node)
{
	return std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node) != m_SelectedNodes.end();
}

const SelectedNodes& CSceneNodesSelector::GetSelectedNodes()
{
	return m_SelectedNodes;
}


//
// IEditor_NodesSelectorInternal
//
void CSceneNodesSelector::SetOtherSelector(IEditor_NodesSelector * OtherSelector)
{
	m_OtherSelector = OtherSelector;
}

void CSceneNodesSelector::SynchronizeWithOtherSelector(IEditor_NodesSelector * OtherSelector)
{
	_ASSERT(OtherSelector != nullptr);
	m_SelectedNodes = OtherSelector->GetSelectedNodes();
	m_SelectionEventsListener->OnSelectNodes();
}



//
// Protected
//
void CSceneNodesSelector::DoSynchronizeWithOtherSelector()
{
	if (m_OtherSelector)
	{
		dynamic_cast<IEditor_NodesSelectorInternal*>(m_OtherSelector)->SynchronizeWithOtherSelector(this);
	}
	else
		_ASSERT(false);
}
