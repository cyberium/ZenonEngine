#include "stdafx.h"

// General
#include "EditorTools.h"

CEditorTools::CEditorTools(IEditor& Editor)
	: m_Editor(Editor)
{
}

CEditorTools::~CEditorTools()
{
}

void CEditorTools::Initialize()
{
	m_Selector = std::make_shared<CEditorToolSelector>(m_Editor);
	m_Selector->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolSelector, m_Selector));

	auto mover = std::make_shared<CEditorToolMover>(m_Editor);
	mover->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolMover, mover));

	auto rotator = std::make_shared<CEditorToolRotator>(m_Editor);
	rotator->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolRotator, rotator));

	auto moverRTS = std::make_shared<CEditorToolMoverRTS>(m_Editor);
	moverRTS->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolMoverRTS, moverRTS));

	auto rotatorRTS = std::make_shared<CEditorToolRotatorRTS>(m_Editor);
	rotatorRTS->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolRotatorRTS, rotatorRTS));

	auto drager = std::make_shared<CEditorToolDragger>(m_Editor);
	drager->Initialize();
	m_Tools.insert(std::make_pair(ETool::EToolDragger, drager));

	m_Editor.GetUIFrame().DoInitializeToolsUI();
	m_Editor.Get3DFrame().DoInitializeTools3D();

	// Default tool
	Enable(ETool::EToolDefault);
}

void CEditorTools::Finalize()
{
}

void CEditorTools::Enable(ETool ToolIndex)
{
	DisableAll(ToolIndex);
	m_Tools.at(ToolIndex)->Enable();
}

IEditorTool& CEditorTools::GetTool(ETool Tool)
{
	auto tool = m_Tools.find(Tool);
	if (tool == m_Tools.end())
		throw CException("Tool %d not found.");

	return *m_Tools[Tool];
}

void CEditorTools::DisableAll(ETool ExceptOfTool)
{
	ETool tool = ExceptOfTool;
	for (const auto& t : m_Tools)
		if (t.first != ExceptOfTool)
			t.second->Disable();
}



//
// 3D
//
void CEditorTools::DoInitialize3D(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	for (const auto& it : m_Tools)
		it.second->DoInitialize3D(RenderTechnique, RenderTarget, Viewport);
}

bool CEditorTools::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			if (it.second->OnMousePressed(e, RayToWorld))
				return true;
	return false;
}

void CEditorTools::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->OnMouseReleased(e, RayToWorld);
}

void CEditorTools::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->OnMouseMoved(e, RayToWorld);
}



//
// UI
//
void CEditorTools::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	for (const auto& it : m_Tools)
		it.second->DoInitializeUI(QtUIFrame);
}

void CEditorTools::DropEvent(const glm::vec2 & Position)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->DropEvent(Position);
}

void CEditorTools::DragEnterEvent(const SDragData & Data)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->DragEnterEvent(Data);
}

void CEditorTools::DragMoveEvent(const glm::vec2 & Position)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->DragMoveEvent(Position);
}

void CEditorTools::DragLeaveEvent()
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->DragLeaveEvent();
}
