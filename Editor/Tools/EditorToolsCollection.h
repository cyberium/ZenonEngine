#pragma once

#include "Editor3DToolBase.h"
#include "Editor3DToolSelector.h"
#include "Editor3DToolMover.h"
#include "Editor3DToolRotator.h"
#include "Editor3DToolDragger.h"

#include "RTS/Editor3DToolMoverRTS.h"
#include "RTS/Editor3DToolRotatorRTS.h"



class CTools
{
public:
	CTools(IEditor3DFrame& Editor3DFrame);
	virtual ~CTools();

	void Initialize();
	void Finalize();
	void Enable(ETool ToolIndex);
	void DisableAll();
	bool OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld);
	void OnMouseReleased(const MouseButtonEventArgs& e, const Ray& RayToWorld);
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld);

public: // Editor
	std::shared_ptr<CEditor3DToolSelector> m_Selector;
	std::shared_ptr<CEditor3DToolMover> m_Mover;
	std::shared_ptr<CEditor3DToolRotator> m_Rotator;
	std::shared_ptr<CEditor3DToolDragger> m_Drager;
	std::shared_ptr<CEditor3DToolMoverRTS> m_MoverRTS;
	std::shared_ptr<CEditor3DToolRotatorRTS> m_RotatorRTS;

private:
	IEditor3DFrame& m_Editor3DFrame;
	std::unordered_map<ETool, std::shared_ptr<CEditor3DToolBase>> m_Tools;
};
