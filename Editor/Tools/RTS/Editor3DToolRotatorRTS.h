#pragma once

#include "../Editor3DToolBase.h"

class CEditor3DToolRotatorRTS
	: public CEditor3DToolBase
{
public:
	CEditor3DToolRotatorRTS(IEditor& Editor);
	virtual ~CEditor3DToolRotatorRTS();

	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;
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
