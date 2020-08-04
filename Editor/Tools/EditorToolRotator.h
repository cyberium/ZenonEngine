#pragma once

#include "EditorToolBase.h"

class CEditorToolRotator
	: public CEditorToolBase
{
public:
	CEditorToolRotator(IEditor& Editor);
	virtual ~CEditorToolRotator();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// CEditorToolBase
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void Clear();

private:
	bool m_IsRotateNow;
	int m_RotatorNuber;
	std::shared_ptr<ISceneNode3D> m_MovingNode;
	glm::vec3 m_MovingObjectPos;
	std::shared_ptr<ISceneNode3D> m_RotatorRoot;
	std::shared_ptr<ISceneNode3D> m_RotatorX;
	std::shared_ptr<ISceneNode3D> m_RotatorY;
	std::shared_ptr<ISceneNode3D> m_RotatorZ;
};
