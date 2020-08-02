#pragma once

#include "Editor3DToolBase.h"
#include "EditorToolSelector3D.h"
#include "EditorToolMover.h"
#include "EditorToolRotator.h"
#include "EditorToolDragger.h"

class CTools
{
public:
	CTools(IEditor3DFrame& Editor3DFrame);
	virtual ~CTools();

	void Initialize();
	void Finalize();
	void Enable(size_t ToolIndex);
	bool OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	void OnMouseMoveToWorld(const MouseMotionEventArgs& e, const Ray& RayToWorld);

public: // Editor
	std::shared_ptr<CEditorToolSelector3D> m_Selector;
	std::shared_ptr<CEditorToolMover> m_Mover;
	std::shared_ptr<CEditorToolRotator> m_Rotator;
	std::shared_ptr<CEditorToolDragger> m_Drager;

private:
	IEditor3DFrame& m_Editor3DFrame;
	std::unordered_map<size_t, std::shared_ptr<CEditor3DToolBase>> m_Tools;
};
