#pragma once

#include "EditorToolBase.h"

class CEditorToolDragger
	: public CEditorToolBase
{
public:
	CEditorToolDragger(IEditor& Editor);
	virtual ~CEditorToolDragger();

	// IEditorTool
	void Enable() override;
	void Disable() override;

	// 3D
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;
	void DropEvent(const glm::vec2& Position) override;
	void DragEnterEvent(const SDragData& Data) override;
	void DragMoveEvent(const glm::vec2& Position) override;
	void DragLeaveEvent() override;

protected: // Editor
	void MoveDraggedNode(const glm::vec2& MousePos);
	void CreateCopyDraggedNode();
	void Clear();

	std::shared_ptr<ISceneNode3D> CreateNode(const glm::ivec3& Position, const std::string& Type);

private:
	bool m_IsDraggingPermanentCreation;
	std::shared_ptr<ISceneNode3D> m_DraggerNode;
	std::shared_ptr<ISceneNode3D> m_LastCreatedNode;
	std::shared_ptr<ISceneNodeUI> m_DraggerTextUI;
};