#pragma once

#include "Tools/EditorToolsCollection.h"
#include "EditorUIFrame.h"
#include "Editor3DFrame.h"

class CEditor
	: public IEditor
	, public IEditorPrivate
	, public IEditor_NodesSelectorEventListener
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

	// Selection part
	bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) const override;
	std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() const override;
	const SelectedNodes& GetSelectedNodes() const override;

	// Scene change part

	// IEditor_NodesSelectorEventListener
	void OnSelectNode() override;

private:
	IBaseManager& m_BaseManager;
	IEditorUIFrame* m_EditorUIFrame;
	IEditor3DFrame* m_Editor3DFrame;

	CTools m_Tools;
};