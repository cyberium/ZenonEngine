#include "stdafx.h"

// General
#include "EditorToolsCollection.h"

CTools::CTools(IEditor3DFrame& Editor3DFrame)
	: m_Editor3DFrame(Editor3DFrame)
{
}

CTools::~CTools()
{
}

void CTools::Initialize()
{
	m_Selector = std::make_shared<CEditor3DToolSelector>(m_Editor3DFrame);
	m_Selector->Initialize();
	m_Tools.insert(std::make_pair(55, m_Selector));

	m_Mover = std::make_shared<CEditor3DToolMover>(m_Editor3DFrame);
	m_Mover->Initialize();
	m_Tools.insert(std::make_pair(30, m_Mover));

	m_Rotator = std::make_shared<CEditor3DToolRotator>(m_Editor3DFrame);
	m_Rotator->Initialize();
	m_Tools.insert(std::make_pair(31, m_Rotator));

	m_MoverRTS = std::make_shared<CEditor3DToolMoverRTS>(m_Editor3DFrame);
	m_MoverRTS->Initialize();
	m_Tools.insert(std::make_pair(32, m_MoverRTS));

	m_RotatorRTS = std::make_shared<CEditor3DToolRotatorRTS>(m_Editor3DFrame);
	m_RotatorRTS->Initialize();
	m_Tools.insert(std::make_pair(33, m_RotatorRTS));


	m_Drager = std::make_shared<CEditor3DToolDragger>(m_Editor3DFrame);
	m_Drager->Initialize();
	m_Tools.insert(std::make_pair(2, m_Drager));
}

void CTools::Finalize()
{
}

void CTools::Enable(ETool ToolIndex)
{
	DisableAll();
	m_Tools.at(ToolIndex)->Enable();
}

void CTools::DisableAll()
{
	for (const auto& t : m_Tools)
		t.second->Disable();
}

bool CTools::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->OnMousePressed(e, RayToWorld));
			return true;
	return false;
}

void CTools::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		it.second->OnMouseReleased(e, RayToWorld);
}

void CTools::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		it.second->OnMouseMoved(e, RayToWorld);
}
