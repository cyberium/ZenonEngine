#pragma once

#include "../EditorToolBase.h"

class CEditorToolMoverRTS
	: public CEditorToolBase
	, public IEditorToolMoverRTS
{
public:
	CEditorToolMoverRTS(IEditor& Editor);
	virtual ~CEditorToolMoverRTS();

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

	// IEditorToolMoverRTS
	glm::vec2 FixBoxCoords(const glm::vec2& Position) override;

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
