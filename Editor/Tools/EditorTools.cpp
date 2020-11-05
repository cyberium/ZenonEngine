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
	auto selector = MakeShared(CEditorToolSelector, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolSelector, selector));

	auto mover = MakeShared(CEditorToolMover, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolMover, mover));

	auto rotator = MakeShared(CEditorToolRotator, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolRotator, rotator));

	auto moverRTS = MakeShared(CEditorToolMoverRTS, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolMoverRTS, moverRTS));

	auto rotatorRTS = MakeShared(CEditorToolRotatorRTS, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolRotatorRTS, rotatorRTS));

	auto drager = MakeShared(CEditorToolDragger, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolDragger, drager));

	auto RTSEditor = MakeShared(CRTSGround, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolEditorRTS, RTSEditor));

	auto editorToolWaypoints = MakeShared(CEditorToolWaypoints, m_Editor);
	m_Tools.insert(std::make_pair(ETool::EToolEditorRTSWaypoints, editorToolWaypoints));

	m_Editor.GetUIFrame().DoInitializeToolsUI();
	m_Editor.Get3DFrame().DoInitializeTools3D();

	// Default tool
	Enable(ETool::EToolDefault);
}

void CEditorTools::Enable(ETool ToolIndex)
{
	DisableAll(ToolIndex);
	m_Tools.at(ToolIndex)->Enable();
}

IEditorTool& CEditorTools::GetTool(ETool Tool)
{
	const auto& tool = m_Tools.find(Tool);
	if (tool == m_Tools.end())
		throw CException("Tool %d not found.", (int)Tool);
	return *(tool->second);
}

const IEditorTool& CEditorTools::GetTool(ETool Tool) const
{
	const auto tool = m_Tools.find(Tool);
	if (tool == m_Tools.end())
		throw CException("Tool %d not found.", (int)Tool);
	return *(tool->second);
}

void CEditorTools::DisableAll(ETool ExceptOfTool)
{
	ETool tool = ExceptOfTool;
	for (const auto& t : m_Tools)
		if (t.first != tool)
			t.second->Disable();
}



//
// 3D
//
void CEditorTools::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	for (const auto& it : m_Tools)
		it.second->DoInitialize3D(Renderer, RenderTarget, Viewport);
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

void CEditorTools::OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode)
{
	for (const auto& it : m_Tools)
		if (it.second->IsEnabled())
			it.second->OnNodeSelected(SelectedNode);
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
