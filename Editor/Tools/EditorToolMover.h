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
	void Enable() override;
	void Disable() override;

	// CEditorToolBase
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
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
	std::shared_ptr<ISceneNode3D> m_MoverRoot;
	std::shared_ptr<ISceneNode3D> m_MoverX;
	std::shared_ptr<ISceneNode3D> m_MoverY;
	std::shared_ptr<ISceneNode3D> m_MoverZ;

	std::weak_ptr<ISceneNode3D> m_MovingNode;

	EMoverDirection m_MoverNumber;
	glm::vec3 m_MoverOffset;
	
	// UI
	float m_MoverValue;
};
