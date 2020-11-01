#pragma once

#include "../EditorToolBase.h"

class CEditorToolRotatorRTS
	: public CEditorToolBase
{
public:
	CEditorToolRotatorRTS(IEditor& Editor);
	virtual ~CEditorToolRotatorRTS();

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

protected:
	void Clear();
	std::shared_ptr<ISceneNode3D> GetRotatingNode();

private:
	bool m_IsRotateNow;
	int m_RotatorNuber;
	std::weak_ptr<ISceneNode3D> m_MovingNode;
	std::shared_ptr<ISceneNode3D> m_RotatorRoot;
	std::shared_ptr<ISceneNode3D> m_RotatorY;
	float m_RotatorInitialAngle;
	int m_LastAbs;
};
