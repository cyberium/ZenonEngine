#include "stdafx.h"

// General
#include "EditorToolsCollection.h"

CTools::CTools(IEditor& Editor)
	: m_Editor(Editor)
{
}

CTools::~CTools()
{
}

void CTools::Initialize()
{
	m_Selector = std::make_shared<CEditor3DToolSelector>(m_Editor);
	m_Selector->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolSelector, m_Selector));

	m_Mover = std::make_shared<CEditor3DToolMover>(m_Editor);
	m_Mover->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolMover, m_Mover));

	m_Rotator = std::make_shared<CEditor3DToolRotator>(m_Editor);
	m_Rotator->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolRotator, m_Rotator));

	m_MoverRTS = std::make_shared<CEditor3DToolMoverRTS>(m_Editor);
	m_MoverRTS->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolMoverRTS, m_MoverRTS));

	m_RotatorRTS = std::make_shared<CEditor3DToolRotatorRTS>(m_Editor);
	m_RotatorRTS->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolRotatorRTS, m_RotatorRTS));


	m_Drager = std::make_shared<CEditor3DToolDragger>(m_Editor);
	m_Drager->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolDragger, m_Drager));

	m_Editor.GetUIFrame().DoInitializeToolsUI();
	m_Editor.Get3DFrame().DoInitializeTools3D();
}

void CTools::Finalize()
{
}

void CTools::Enable(ETool ToolIndex)
{
	DisableAll();
	m_Tools.at(ToolIndex)->Enable();
}

IEditorTool& CTools::GetTool(ETool Tool)
{
	auto tool = m_Tools.find(Tool);
	if (tool == m_Tools.end())
		throw CException("Tool %d not found.");

	return *m_Tools[Tool];
}

void CTools::DisableAll()
{
	for (const auto& t : m_Tools)
		t.second->Disable();
}



//
// 3D
//
void CTools::AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	for (const auto& it : m_Tools)
		it.second->AddPasses(RenderTechnique, RenderTarget, Viewport);
}

bool CTools::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->OnMousePressed(e, RayToWorld))
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



//
// UI
//
void CTools::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	for (const auto& it : m_Tools)
		it.second->DoInitializeUI(QtUIFrame);
}

void CTools::DropEvent(const glm::vec2 & Position)
{
	for (const auto& it : m_Tools)
		it.second->DropEvent(Position);
}

void CTools::DragEnterEvent(const SDragData & Data)
{
	for (const auto& it : m_Tools)
		it.second->DragEnterEvent(Data);
}

void CTools::DragMoveEvent(const glm::vec2 & Position)
{
	for (const auto& it : m_Tools)
		it.second->DragMoveEvent(Position);
}

void CTools::DragLeaveEvent()
{
	for (const auto& it : m_Tools)
		it.second->DragLeaveEvent();
}
