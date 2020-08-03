#include "stdafx.h"

// General
#include "EditorUIToolBase.h"

CEditorUIToolBase::CEditorUIToolBase(IEditor3DFrame& EditorFrame)
	: m_Editor3DFrame(EditorFrame)
	, m_IsEnabled(false)
{
}

CEditorUIToolBase::~CEditorUIToolBase()
{
}



//
// IEditorTool
//
void CEditorUIToolBase::Enable()
{
	m_IsEnabled = true;
}

void CEditorUIToolBase::Disable()
{
	m_IsEnabled = false;
}

bool CEditorUIToolBase::IsEnabled() const
{
	return m_IsEnabled;
}

IEditorUIFrame& CEditorUIToolBase::GetEditorUIFrame() const
{
	return m_EditorUIFrame;
}

IScene * CEditorUIToolBase::GetScene() const
{
	if (auto scene = m_EditorUIFrame.GetScene())
		return scene.get();
	return nullptr;
}

IBaseManager & CEditorUIToolBase::GetBaseManager() const
{
	if (auto scene = m_EditorUIFrame.GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager();
	throw CException("Scene is expired.");
}

IRenderDevice & CEditorUIToolBase::GetRenderDevice() const
{
	if (auto scene = m_EditorUIFrame.GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager().GetApplication().GetRenderDevice();
	throw CException("Scene is expired.");
}
