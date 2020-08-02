#pragma once

#include "Editor3DToolBase.h"

class CEditorToolRotator
	: public CEditor3DToolBase
{
public:
	CEditorToolRotator(IEditor3DFrame& EditorFrame);
	virtual ~CEditorToolRotator();

	void Initialize();
	void Finalize();
	void Enable();
	void Disable();
	bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld);

protected:
	void Clear();

private:
	bool m_IsMoverEnable;
	int m_MoverNuber;
	std::shared_ptr<ISceneNode3D> m_MovingNode;
	glm::vec3 m_MovingObjectPos;
	std::shared_ptr<ISceneNode3D> m_MoverRoot;
	glm::vec3 m_MoverOffset;
	std::shared_ptr<ISceneNode3D> m_MoverX;
	std::shared_ptr<ISceneNode3D> m_MoverY;
	std::shared_ptr<ISceneNode3D> m_MoverZ;
};
