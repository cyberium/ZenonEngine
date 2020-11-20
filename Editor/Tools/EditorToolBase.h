#pragma once

#include "EditorQtInterfaces.h"

class CEditorToolBase
	: public IEditorTool
	, public IEditorToolUI
{
public:
	CEditorToolBase(IEditor& Editor);
	virtual ~CEditorToolBase();

	// IEditorTool
	virtual void Enable() override;
	virtual void Disable() override;
	bool IsEnabled() const override final;

	// 3D
	virtual void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget);
	virtual bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode) {}

	// IEditorToolUI
	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;
	virtual void DropEvent(const glm::vec2& Position) override;
	virtual void DragEnterEvent(const SDragData& Data) override;
	virtual void DragMoveEvent(const glm::vec2& Position) override;
	virtual void DragLeaveEvent();

protected:
	IEditor& GetEditor() const;
	IScene& GetScene() const;
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	IEditor& m_Editor;
	bool m_IsEnabled;
};