#pragma once

#include "../Editor3DToolBase.h"

class CEditor3DToolMoverRTS
	: public CEditor3DToolBase
{
public:
	CEditor3DToolMoverRTS(IEditor3DFrame& EditorFrame);
	virtual ~CEditor3DToolMoverRTS();

	// // CEditor3DToolBase
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	glm::ivec3 ToBoxCoords(const glm::vec3 & Position);
	glm::vec3 FixBoxCoords(const glm::vec3 & Position);
	void SetMoverValue(float Value);

protected:
	void Clear();

private:
	float m_MoverValue;
	bool m_IsMovingNow;
	int m_MoverNuber;
	std::shared_ptr<ISceneNode3D> m_MovingNode;
	glm::vec3 m_MovingObjectPos;
	std::shared_ptr<ISceneNode3D> m_MoverRoot;
	glm::vec3 m_MoverOffset;
	std::shared_ptr<ISceneNode3D> m_MoverX;
	std::shared_ptr<ISceneNode3D> m_MoverZ;
};
