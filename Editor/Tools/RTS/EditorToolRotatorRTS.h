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
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void Clear();
	std::shared_ptr<ISceneNode> GetRotatingNode();

private:
	std::shared_ptr<ISceneNode> m_RotatorRoot;
	std::shared_ptr<ISceneNode> m_RotatorY;

	std::weak_ptr<ISceneNode>   m_RotatingNode;

	EMoverDirection               m_RotatorNumber;

	float                         m_InitialRotationRadians;
	int                           m_StartMousePosY;
};
