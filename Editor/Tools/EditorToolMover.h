#pragma once

#include "Editor3DToolBase.h"

class CEditorToolMover
	: public CEditor3DToolBase
{
public:
	CEditorToolMover(IEditor3DFrame& EditorFrame);
	virtual ~CEditorToolMover();

	void Initialize();
	void Finalize();
	void Enable();
	void Disable();
	bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld);

	glm::ivec3 ToBoxCoords(const glm::vec3 & Position);
	glm::vec3 FixBoxCoords(const glm::vec3 & Position);
	void SetMoverValue(float Value);

protected:
	void Clear();

private:
	float m_MoverValue;
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
