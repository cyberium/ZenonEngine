#pragma once

#include "Editor3DToolBase.h"

class CEditorToolDragger
	: public CEditor3DToolBase
{
public:
	CEditorToolDragger(IEditor3DFrame& EditorFrame);
	virtual ~CEditorToolDragger();

	void Initialize();
	void Finalize();
	void Enable();
	void Disable();
	bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld);

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