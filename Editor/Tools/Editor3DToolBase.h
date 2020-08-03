#pragma once

#include "EditorQtInterfaces.h"

class CEditor3DToolBase
	: public IEditorTool
	, public IEditorToolUI
{
public:
	CEditor3DToolBase(IEditor& Editor);
	virtual ~CEditor3DToolBase();

	// IEditorTool
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Enable() override;
	virtual void Disable() override;
	bool IsEnabled() const override final;

	// 3D
	virtual void AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport);
	virtual bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld) = 0;
	virtual void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) = 0;

	// IEditorToolUI
	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;
	virtual void DropEvent(const glm::vec2& Position) override;
	virtual void DragEnterEvent(const SDragData& Data) override;
	virtual void DragMoveEvent(const glm::vec2& Position) override;
	virtual void DragLeaveEvent();

protected:
	IEditor& GetEditor() const;
	IScene* GetScene() const;
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;

private:
	IEditor& m_Editor;
	bool m_IsEnabled;
};