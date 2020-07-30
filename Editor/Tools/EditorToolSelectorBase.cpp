#include "stdafx.h"

// General
#include "EditorToolSelectorBase.h"

CEditorToolSelector::CEditorToolSelector(IEditor_NodesSelectorEventListener& NodesSelectorEventListener)
	: m_NodesSelectorEventListener(NodesSelectorEventListener)
	, m_OtherSelector(nullptr)
{
}

CEditorToolSelector::~CEditorToolSelector()
{
}



void CEditorToolSelector::SelectNode(std::shared_ptr<ISceneNode3D> Node)
{
	m_SelectedNodes.clear();
	m_SelectedNodes.push_back(Node);

	DoSynchronizeWithOtherSelector();
	RaiseSelectEvent();
}

void CEditorToolSelector::SelectNodes(const SelectedNodes & Nodes)
{
	m_SelectedNodes = Nodes;

	DoSynchronizeWithOtherSelector();
	RaiseSelectEvent();
}

void CEditorToolSelector::ClearSelection()
{
	m_SelectedNodes.clear();

	DoSynchronizeWithOtherSelector();
	RaiseSelectEvent();
}

void CEditorToolSelector::AddNode(std::shared_ptr<ISceneNode3D> Node)
{
	if (std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node) != m_SelectedNodes.end())
		return;
	m_SelectedNodes.push_back(Node);

	DoSynchronizeWithOtherSelector();
	RaiseSelectEvent();
}

void CEditorToolSelector::RemoveNode(std::shared_ptr<ISceneNode3D> Node)
{
	auto it = std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node);
	if (it == m_SelectedNodes.end())
		return;
	m_SelectedNodes.erase(it);

	DoSynchronizeWithOtherSelector();
	RaiseSelectEvent();
}

std::shared_ptr<ISceneNode3D> CEditorToolSelector::GetFirstSelectedNode()
{
	auto first = m_SelectedNodes.begin();
	if (first == m_SelectedNodes.end())
		return nullptr;

	return *first;
}

bool CEditorToolSelector::IsNodeSelected(std::shared_ptr<ISceneNode3D> Node)
{
	return std::find(m_SelectedNodes.begin(), m_SelectedNodes.end(), Node) != m_SelectedNodes.end();
}

const SelectedNodes& CEditorToolSelector::GetSelectedNodes()
{
	return m_SelectedNodes;
}


//
// IEditor_NodesSelectorInternal
//
void CEditorToolSelector::SetOtherSelector(IEditor_NodesSelector * OtherSelector)
{
	m_OtherSelector = OtherSelector;
}

void CEditorToolSelector::SynchronizeWithOtherSelector(IEditor_NodesSelector * OtherSelector)
{
	_ASSERT(OtherSelector != nullptr);
	m_SelectedNodes = OtherSelector->GetSelectedNodes();
	RaiseSelectEvent();
}



//
// Protected
//
void CEditorToolSelector::RaiseSelectEvent()
{
	m_NodesSelectorEventListener.OnSelectNodes();
}

void CEditorToolSelector::DoSynchronizeWithOtherSelector()
{
	if (m_OtherSelector)
	{
		dynamic_cast<IEditor_NodesSelectorInternal*>(m_OtherSelector)->SynchronizeWithOtherSelector(this);
	}
	else
		_ASSERT(false);
}
