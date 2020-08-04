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
void CEditorToolBase::DoInitialize3D(RenderTechnique & RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
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

IScene * CEditorToolBase::GetScene() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return scene.get();
	return nullptr;
}

IBaseManager & CEditorToolBase::GetBaseManager() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager();
	throw CException("Scene is expired.");
}

IRenderDevice & CEditorToolBase::GetRenderDevice() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager().GetApplication().GetRenderDevice();
	throw CException("Scene is expired.");
}


