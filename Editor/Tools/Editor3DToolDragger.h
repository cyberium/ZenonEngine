#pragma once

#include "Editor3DToolBase.h"

class CEditor3DToolDragger
	: public CEditor3DToolBase
{
public:
	CEditor3DToolDragger(IEditor& Editor);
	virtual ~CEditor3DToolDragger();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// 3D
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
	void DoMoveNode(const glm::vec2& MousePos);
	void DoDropNodeAndCreateIt();
	std::shared_ptr<ISceneNode3D> CreateNode(const glm::ivec3& Position, const std::string& Type);

private:
	bool m_IsDraggingEnabled;
	bool m_IsDraggingPermanentCreation;
	std::shared_ptr<ISceneNode3D> m_DraggedNode;
	std::shared_ptr<ISceneNodeUI> m_DraggerTextUI;
};