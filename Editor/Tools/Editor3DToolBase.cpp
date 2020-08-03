#include "stdafx.h"

// General
#include "Editor3DToolBase.h"

CEditor3DToolBase::CEditor3DToolBase(IEditor& Editor)
	: m_Editor(Editor)
	, m_IsEnabled(false)
{
}

CEditor3DToolBase::~CEditor3DToolBase()
{
}



//
// IEditorTool
//
void CEditor3DToolBase::Enable()
{
	m_IsEnabled = true;
}

void CEditor3DToolBase::Disable()
{
	m_IsEnabled = false;
}

bool CEditor3DToolBase::IsEnabled() const
{
	return m_IsEnabled;
}



//
// 3D
//
void CEditor3DToolBase::AddPasses(RenderTechnique & RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
}

//
// IEditorToolUI
//
void CEditor3DToolBase::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
}

void CEditor3DToolBase::DropEvent(const glm::vec2 & Position)
{
}

void CEditor3DToolBase::DragEnterEvent(const SDragData & Data)
{
}

void CEditor3DToolBase::DragMoveEvent(const glm::vec2 & Position)
{
}

void CEditor3DToolBase::DragLeaveEvent()
{
}



//
// Protected
//
IEditor& CEditor3DToolBase::GetEditor() const
{
	return m_Editor;
}

IScene * CEditor3DToolBase::GetScene() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return scene.get();
	return nullptr;
}

IBaseManager & CEditor3DToolBase::GetBaseManager() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager();
	throw CException("Scene is expired.");
}

IRenderDevice & CEditor3DToolBase::GetRenderDevice() const
{
	if (auto scene = m_Editor.Get3DFrame().GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager().GetApplication().GetRenderDevice();
	throw CException("Scene is expired.");
}


