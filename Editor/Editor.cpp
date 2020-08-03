#include "stdafx.h"

// General
#include "Editor.h"

CEditor::CEditor(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
	, m_Tools(*this)
{
}

CEditor::~CEditor()
{
}

IBaseManager & CEditor::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice & CEditor::GetRenderDevice() const
{
	return m_BaseManager.GetApplication().GetRenderDevice();
}

IEditorUIFrame& CEditor::GetUIFrame() const
{
	return *m_EditorUIFrame;
}

IEditor3DFrame & CEditor::Get3DFrame() const
{
	return *m_Editor3DFrame;
}

IEditorTools& CEditor::GetTools()
{
	return m_Tools;
}

bool CEditor::IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) const
{
	return m_Tools.m_Selector->IsNodeSelected(Node);
}

std::shared_ptr<ISceneNode3D> CEditor::GetFirstSelectedNode() const
{
	return m_Tools.m_Selector->GetFirstSelectedNode();
}

const SelectedNodes& CEditor::GetSelectedNodes() const
{
	return m_Tools.m_Selector->GetSelectedNodes();
}



//
// IEditor_NodesSelectorEventListener
//
void CEditor::OnSelectNode()
{
	dynamic_cast<IEditor_NodesSelectorEventListener*>(m_Editor3DFrame)->OnSelectNode();
	dynamic_cast<IEditor_NodesSelectorEventListener*>(m_EditorUIFrame)->OnSelectNode();
}
