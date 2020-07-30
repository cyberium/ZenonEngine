#include "stdafx.h"

// General
#include "Editor3DToolBase.h"

CEditor3DToolBase::CEditor3DToolBase(IEditor3DFrame& EditorFrame)
	: m_Editor3DFrame(EditorFrame)
	, m_IsEnabled(false)
{
}

CEditor3DToolBase::~CEditor3DToolBase()
{
}

void CEditor3DToolBase::Enable()
{
	m_IsEnabled = true;
}

bool CEditor3DToolBase::IsEnabled()
{
	return m_IsEnabled;
}

void CEditor3DToolBase::Disable()
{
	m_IsEnabled = false;
}

IEditor3DFrame& CEditor3DToolBase::GetEditor3DFrame() const
{
	return m_Editor3DFrame;
}

IScene * CEditor3DToolBase::GetScene() const
{
	if (auto scene = m_Editor3DFrame.GetScene())
		return scene.get();
	return nullptr;
}

IBaseManager & CEditor3DToolBase::GetBaseManager() const
{
	if (auto scene = m_Editor3DFrame.GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager();
	throw CException("Scene is expired.");
}

IRenderDevice & CEditor3DToolBase::GetRenderDevice() const
{
	if (auto scene = m_Editor3DFrame.GetScene())
		return std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager().GetApplication().GetRenderDevice();
	throw CException("Scene is expired.");
}
