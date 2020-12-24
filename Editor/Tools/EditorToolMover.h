#pragma once

#include "EditorToolBase.h"

class CEditorToolMover
	: public CEditorToolBase
{
public:
	CEditorToolMover(IEditor& Editor);
	virtual ~CEditorToolMover();

	// IEditorTool
	void Enable() override;
	void Disable() override;

	// CEditorToolBase
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;
	void OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void Clear();
	std::shared_ptr<ISceneNode> GetMovingNode();

private:
	std::shared_ptr<ISceneNode> m_MoverRoot;
	std::shared_ptr<ISceneNode> m_MoverX;
	std::shared_ptr<ISceneNode> m_MoverY;
	std::shared_ptr<ISceneNode> m_MoverZ;

	std::weak_ptr<ISceneNode>   m_MovingNode;

	EMoverDirection             m_MoverNumber;
	glm::vec3                   m_MoverOffset;

};
