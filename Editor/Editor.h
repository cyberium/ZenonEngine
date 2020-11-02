#pragma once

#include "Tools/EditorTools.h"
#include "EditorWindowsShell.h"
#include "EditorUIFrame.h"
#include "Editor3DFrame.h"

class CEditor
	: public IEditor
	, public IEditorPrivate
	, public IEditorToolSelectorEventListener
{
public:
	CEditor(IBaseManager& BaseManager);
	virtual ~CEditor();

	// IEditorPrivate
	void SetUIFrame(IEditorUIFrame* EditorUIFrame) override final { m_EditorUIFrame = EditorUIFrame; }
	void Set3DFrame(IEditor3DFrame* Editor3DFrame) override final { m_Editor3DFrame = Editor3DFrame; }

	// IEditor
	IBaseManager& GetBaseManager() const override;
	IRenderDevice& GetRenderDevice() const override;
	IEditorUIFrame& GetUIFrame() const override;
	IEditor3DFrame& Get3DFrame() const override;
	IEditorTools& GetTools() override;
	IEditorShell& GetShell() override;

	// Selection part
	bool IsNodeSelected(std::shared_ptr<ISceneNode> Node) const override;
	std::shared_ptr<ISceneNode> GetFirstSelectedNode() const override;
	std::vector<std::shared_ptr<ISceneNode>> GetSelectedNodes() const override;

	// Scene change part

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

private:
	IBaseManager& m_BaseManager;
	IEditorUIFrame* m_EditorUIFrame;
	IEditor3DFrame* m_Editor3DFrame;

	CEditorTools m_Tools;
	CEditorWindowsShell m_EditorShell;
};