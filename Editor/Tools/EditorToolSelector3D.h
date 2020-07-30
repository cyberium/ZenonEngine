#pragma once

#include "Editor3DToolBase.h"
#include "EditorToolSelectorBase.h"
#include "Passes/DrawSelectionPass.h"

class CEditorToolSelector3D
	: public CEditorToolSelector
	, public CEditor3DToolBase
{
public:
	CEditorToolSelector3D(IEditor3DFrame& EditorFrame);
	virtual ~CEditorToolSelector3D();

	// CEditor3DToolBase
	void Initialize();
	void Finalize();
	void Enable();
	void Disable();
	bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld);

	// 
	void AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport);

protected:
	void RaiseSelectEvent();

private:
	bool m_IsSelecting2D;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<ISceneNodeUI> m_SelectionTexture;
	std::shared_ptr<CDrawSelectionPass> m_DrawSelectionPass;
};
