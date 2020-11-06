#pragma once

#include "Tools/EditorToolBase.h"

class CEditorToolWaypoints
	: public CEditorToolBase
{
public:
	CEditorToolWaypoints(IEditor& Editor);
	virtual ~CEditorToolWaypoints();

	// IEditorTool
	void Enable() override;
	void Disable() override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

	// 3D
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;
	void OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode) override;

protected:
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;
	void AddPoint(glm::vec3 XYZ);

private:
	std::shared_ptr<ISceneNodeRTSPath> m_CurrectWaypointsNode;
	std::shared_ptr<ISceneNodeRTSPoint> m_LastCreatedPoint;
	bool m_IsActivated;

private:
	IBaseManager& m_BaseManager;
};