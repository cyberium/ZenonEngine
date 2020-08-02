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
	m_Selector = std::make_shared<CEditorToolSelector3D>(m_Editor3DFrame);
	m_Selector->Initialize();
	m_Tools.insert(std::make_pair(55, m_Selector));

	m_Mover = std::make_shared<CEditorToolMover>(m_Editor3DFrame);
	m_Mover->Initialize();
	m_Tools.insert(std::make_pair(30, m_Mover));

	m_Rotator = std::make_shared<CEditorToolRotator>(m_Editor3DFrame);
	m_Rotator->Initialize();
	m_Tools.insert(std::make_pair(31, m_Rotator));


	m_Drager = std::make_shared<CEditorToolDragger>(m_Editor3DFrame);
	m_Drager->Initialize();
	m_Tools.insert(std::make_pair(2, m_Drager));
}

void CTools::Finalize()
{
}

void CTools::Enable(size_t ToolIndex)
{
	for (const auto& t : m_Tools)
		t.second->Disable();

	m_Tools.at(ToolIndex)->Enable();
}

bool CTools::OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->OnMouseClickToWorld(e, RayToWorld));
			return true;
	return false;
}

void CTools::OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		it.second->OnMouseReleaseToWorld(e, RayToWorld);
}

void CTools::OnMouseMoveToWorld(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		it.second->OnMouseMoveToWorld(e, RayToWorld);
}
