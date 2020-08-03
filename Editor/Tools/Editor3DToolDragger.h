#pragma once

#include "Editor3DToolBase.h"

class CEditor3DToolDragger
	: public CEditor3DToolBase
{
public:
	CEditor3DToolDragger(IEditor3DFrame& EditorFrame);
	virtual ~CEditor3DToolDragger();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// CEditor3DToolBase
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	void DropEvent(const glm::vec2& Position);
	void DragEnterEvent(const SDragData& Data);
	void DragMoveEvent(const glm::vec2& Position);
	void DragLeaveEvent();

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