#pragma once

#include "Tools/EditorTools.h"
#include "EditorWindowsShell.h"

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
	void Set3DPreviewFrame(IEditor3DPreviewFrame* Editor3DPreviewFrame) override final { m_Editor3DPreviewFrame = Editor3DPreviewFrame; }

	// IEditor
	IBaseManager& GetBaseManager() const override;
	IRenderDevice& GetRenderDevice() const override;
	IEditorUIFrame& GetUIFrame() const override;
	IEditor3DFrame& Get3DFrame() const override;
	IEditor3DPreviewFrame& Get3DPreviewFrame() const override;
	IEditorTools& GetTools() override;
	IEditorShell& GetShell() override;

	// Selection part
	bool IsNodeSelected(std::shared_ptr<ISceneNode> Node) const override;
	std::shared_ptr<ISceneNode> GetFirstSelectedNode() const override;
	std::vector<std::shared_ptr<ISceneNode>> GetSelectedNodes() const override;

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

private:
	IBaseManager& m_BaseManager;
	IEditorUIFrame* m_EditorUIFrame;
	IEditor3DFrame* m_Editor3DFrame;
	IEditor3DPreviewFrame* m_Editor3DPreviewFrame;
	CEditorTools m_Tools;
	CEditorWindowsShell m_EditorShell;
};