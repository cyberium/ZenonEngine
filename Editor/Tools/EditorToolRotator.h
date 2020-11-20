#pragma once

#include "EditorToolBase.h"

class CEditorToolRotator
	: public CEditorToolBase
	, public IEditorToolRotator
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

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

	// IEditorToolRotator
	float FixAngle(float Angle) override;
	void SetRotatorValue(float Value) override;
	float GetRotatorValue() const override;

protected:
	void Clear();
	std::shared_ptr<ISceneNode> GetRotatingNode();

private:	
	std::shared_ptr<ISceneNode> m_RotatorRoot;
	std::shared_ptr<ISceneNode> m_RotatorX;
	std::shared_ptr<ISceneNode> m_RotatorY;
	std::shared_ptr<ISceneNode> m_RotatorZ;

	std::weak_ptr<ISceneNode> m_RotatingNode;

	EMoverDirection m_RotatorNumber;

	// UI
	float m_RotatorValue;
};
