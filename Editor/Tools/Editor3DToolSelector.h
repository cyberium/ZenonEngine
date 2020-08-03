#pragma once

#include "Editor3DToolBase.h"
#include "EditorToolSelectorBase.h"
#include "Passes/DrawSelectionPass.h"

class CEditor3DToolSelector
	: public CEditorToolSelector
	, public CEditor3DToolBase
{
public:
	CEditor3DToolSelector(IEditor& Editor);
	virtual ~CEditor3DToolSelector();

	// IEditorTool
	void Initialize() override;
	void Finalize() override;
	void Enable() override;
	void Disable() override;

	// CEditor3DToolBase
	void AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void RaiseSelectEvent();

private:
	bool m_IsSelecting2D;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<ISceneNodeUI> m_SelectionTexture;
	std::shared_ptr<CDrawSelectionPass> m_DrawSelectionPass;
};
