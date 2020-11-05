#pragma once

#include "EditorToolBase.h"
#include "EditorToolSelector.h"
#include "EditorToolMover.h"
#include "EditorToolRotator.h"
#include "EditorToolDragger.h"

#include "RTS/EditorToolMoverRTS.h"
#include "RTS/EditorToolRotatorRTS.h"
#include "RTS/EditorToolRTSGround.h"
#include "RTS/EditorToolWaypoints.h"


class CEditorTools
	: public IEditorTools
{
public:
	CEditorTools(IEditor& Editor);
	virtual ~CEditorTools();

	// IEditorTools
	void Initialize() override;
	void Enable(ETool ToolIndex) override;
	IEditorTool& GetTool(ETool Tool) override;
	const IEditorTool& GetTool(ETool Tool) const override;
	void DisableAll(ETool ExceptOfTool = EToolDefault) override;

	// 3D
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;
	void OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode) override;

	// UI
	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;
	virtual void DropEvent(const glm::vec2& Position) override;
	virtual void DragEnterEvent(const SDragData& Data) override;
	virtual void DragMoveEvent(const glm::vec2& Position) override;
	virtual void DragLeaveEvent() override;

private:
	IEditor& m_Editor;
	std::map<ETool, std::shared_ptr<CEditorToolBase>> m_Tools;
};
