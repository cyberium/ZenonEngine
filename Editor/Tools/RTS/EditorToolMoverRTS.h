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
	void Enable() override;
	void Disable() override;

	// 3D
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
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
	
	std::shared_ptr<ISceneNode3D> m_MovingNode;

	std::shared_ptr<ISceneNode3D> m_MoverRoot;
	std::shared_ptr<ISceneNode3D> m_MoverX;
	std::shared_ptr<ISceneNode3D> m_MoverZ;

	bool m_IsMovingNow;
	int m_MoverNuber;
	glm::vec3 m_MoverOffset;
};
