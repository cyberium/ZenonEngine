#pragma once

#include "EditorToolBase.h"

class CEditorToolMover
	: public CEditorToolBase
	, public IEditorToolMover
{
public:
	CEditorToolMover(IEditor& Editor);
	virtual ~CEditorToolMover();

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

	// IEditorToolMover
	glm::vec3 FixBoxCoords(const glm::vec3 & Position) override;
	void SetMoverValue(float Value) override;
	float GetMoverValue() const override;

protected:
	void Clear();
	std::shared_ptr<ISceneNode3D> GetMovingNode();

private:
	float m_MoverValue;
	bool m_IsMovingNow;
	int m_MoverNuber;
	std::weak_ptr<ISceneNode3D> m_MovingNode;
	glm::vec3 m_MovingObjectPos;
	std::shared_ptr<ISceneNode3D> m_MoverRoot;
	glm::vec3 m_MoverOffset;
	std::shared_ptr<ISceneNode3D> m_MoverX;
	std::shared_ptr<ISceneNode3D> m_MoverY;
	std::shared_ptr<ISceneNode3D> m_MoverZ;

	// UI
	std::map<std::string, float> m_MoverValues;
};
