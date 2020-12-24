#pragma once

#include "EditorToolBase.h"

class CEditorToolRotator
	: public CEditorToolBase
{
public:
	CEditorToolRotator(IEditor& Editor);
	virtual ~CEditorToolRotator();

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
	std::shared_ptr<ISceneNode> GetRotatingNode();

private:	
	std::shared_ptr<ISceneNode> m_RotatorRoot;
	std::shared_ptr<ISceneNode> m_RotatorX;
	std::shared_ptr<ISceneNode> m_RotatorY;
	std::shared_ptr<ISceneNode> m_RotatorZ;

	std::weak_ptr<ISceneNode>   m_RotatingNode;
	EMoverDirection             m_RotatorNumber;

	glm::vec3                   m_InitialRotationDegrees;
	int                         m_StartMousePosY;
};
