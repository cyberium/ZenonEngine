#pragma once

#include "EditorToolBase.h"
#include "Passes/DrawSelectionPass.h"

class CEditorToolSelector
	: public IEditorToolSelector
	, public CEditorToolBase
{
public:
	CEditorToolSelector(IEditor& Editor);
	virtual ~CEditorToolSelector();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// IEditorToolSelector
	void SelectNode(std::shared_ptr<ISceneNode3D> Node) override;
	void SelectNodes(const std::vector<std::shared_ptr<ISceneNode3D>>& Nodes) override;
	void ClearSelection() override;
	void AddNode(std::shared_ptr<ISceneNode3D> Node) override;
	void RemoveNode(std::shared_ptr<ISceneNode3D> Node) override;
	std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() override;
	bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) override;
	const SelectedNodes& GetSelectedNodes() override;

	// CEditorToolBase
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void RaiseSelectEvent();

private:
	SelectedNodes m_SelectedNodes;
	IEditorToolSelectorEventListener& m_NodesSelectorEventListener;

	bool m_IsSelecting2D;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<ISceneNodeUI> m_SelectionTexture;
	std::shared_ptr<CDrawSelectionPass> m_DrawSelectionPass;
};
