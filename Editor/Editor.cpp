#include "stdafx.h"

// General
#include "Editor.h"

CEditor::CEditor(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
	, m_Tools(*this)
	, m_EditorShell(*this)
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

IEditorShell & CEditor::GetShell()
{
	return m_EditorShell;
}

bool CEditor::IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) const
{
	return m_Tools.m_Selector->IsNodeSelected(Node);
}

std::shared_ptr<ISceneNode3D> CEditor::GetFirstSelectedNode() const
{
	return m_Tools.m_Selector->GetFirstSelectedNode();
}

std::vector<std::shared_ptr<ISceneNode3D>> CEditor::GetSelectedNodes() const
{
	std::vector<std::shared_ptr<ISceneNode3D>> nodes;
	for (const auto& nW : m_Tools.m_Selector->GetSelectedNodes())
		if (auto locked = nW.lock())
			nodes.push_back(locked);
	return nodes;
}



//
// IEditorToolSelectorEventListener
//
void CEditor::OnSelectNode()
{
	dynamic_cast<IEditorToolSelectorEventListener*>(m_Editor3DFrame)->OnSelectNode();
	dynamic_cast<IEditorToolSelectorEventListener*>(m_EditorUIFrame)->OnSelectNode();
}
