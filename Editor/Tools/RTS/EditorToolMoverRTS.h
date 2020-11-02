#pragma once

#include "../EditorToolBase.h"

class CEditorToolMoverRTS
	: public CEditorToolBase
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

protected:
	void Clear();
	std::shared_ptr<ISceneNode> GetMovingNode();

private:
	std::shared_ptr<ISceneNode> m_MoverRoot;
	std::shared_ptr<ISceneNode> m_MoverX;
	std::shared_ptr<ISceneNode> m_MoverZ;

	std::weak_ptr<ISceneNode>   m_MovingNode;

	EMoverDirection               m_MoverNumber;
	glm::vec3                     m_MoverOffset;
};
