#include "stdafx.h"

// General
#include "EditorToolBase.h"

CEditorToolBase::CEditorToolBase(IEditor& Editor)
	: m_Editor(Editor)
	, m_IsEnabled(false)
{
}

CEditorToolBase::~CEditorToolBase()
{
}



//
// IEditorTool
//
void CEditorToolBase::Enable()
{
	m_IsEnabled = true;
}

void CEditorToolBase::Disable()
{
	m_IsEnabled = false;
}

bool CEditorToolBase::IsEnabled() const
{
	return m_IsEnabled;
}



//
// 3D
//
void CEditorToolBase::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
}



//
// IEditorToolUI
//
void CEditorToolBase::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
}

void CEditorToolBase::DropEvent(const glm::vec2 & Position)
{
}

void CEditorToolBase::DragEnterEvent(const SDragData & Data)
{
}

void CEditorToolBase::DragMoveEvent(const glm::vec2 & Position)
{
}

void CEditorToolBase::DragLeaveEvent()
{
}



//
// Protected
//
IEditor& CEditorToolBase::GetEditor() const
{
	return m_Editor;
}

IScene& CEditorToolBase::GetScene() const
{
	return m_Editor.Get3DFrame().GetScene();
}

IBaseManager& CEditorToolBase::GetBaseManager() const
{
	return m_Editor.Get3DFrame().GetScene().GetBaseManager();
}

IRenderDevice& CEditorToolBase::GetRenderDevice() const
{
	return m_Editor.Get3DFrame().GetScene().GetRenderDevice();
}


