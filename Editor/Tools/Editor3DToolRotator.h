#pragma once

#include "Editor3DToolBase.h"

class CEditor3DToolRotator
	: public CEditor3DToolBase
{
public:
	CEditor3DToolRotator(IEditor3DFrame& EditorFrame);
	virtual ~CEditor3DToolRotator();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// CEditor3DToolBase
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

protected:
	void Clear();

private:
	bool m_IsMoverEnable;
	int m_RotatorNuber;
	std::shared_ptr<ISceneNode3D> m_MovingNode;
	glm::vec3 m_MovingObjectPos;
	std::shared_ptr<ISceneNode3D> m_RotatorRoot;
	std::shared_ptr<ISceneNode3D> m_RotatorX;
	std::shared_ptr<ISceneNode3D> m_RotatorY;
	std::shared_ptr<ISceneNode3D> m_RotatorZ;
};
