#include "stdafx.h"

// General
#include "NativeWindowProxyBase.h"

CNativeWindowProxyBase::CNativeWindowProxyBase(IznNativeWindow& SourceNativeWindow)
	: m_SourceNativeWindow(SourceNativeWindow)
	, m_EventListener(nullptr)
{
	m_SourceNativeWindow.SetEventsListener(this);
}

CNativeWindowProxyBase::~CNativeWindowProxyBase()
{
	m_SourceNativeWindow.ResetEventsListener();
}



//
// IznNativeWindow
//
void CNativeWindowProxyBase::SetWindowTitle(const std::string& WindowName)
{
	m_SourceNativeWindow.SetWindowTitle(WindowName);
}

std::string CNativeWindowProxyBase::GetWindowTitle() const
{
	return m_SourceNativeWindow.GetWindowTitle();
}

size_t CNativeWindowProxyBase::GetWindowWidth() const
{
	return m_SourceNativeWindow.GetWindowWidth();
}

size_t CNativeWindowProxyBase::GetWindowHeight() const
{
	return m_SourceNativeWindow.GetWindowHeight();
}

void CNativeWindowProxyBase::SetCursorPosition(const glm::ivec2& CursorPosition)
{
	m_SourceNativeWindow.SetCursorPosition(CursorPosition);
}

glm::ivec2 CNativeWindowProxyBase::GetCursorPosition() const
{
	return m_SourceNativeWindow.GetCursorPosition();
}

void CNativeWindowProxyBase::ShowCursor()
{
	m_SourceNativeWindow.ShowCursor();
}

void CNativeWindowProxyBase::HideCursor()
{
	m_SourceNativeWindow.HideCursor();
}

void CNativeWindowProxyBase::Close()
{
	m_SourceNativeWindow.Close();
}

void CNativeWindowProxyBase::SetEventsListener(IznNativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void CNativeWindowProxyBase::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// IznNativeWindowEventListener
//
void CNativeWindowProxyBase::OnWindowInputFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowInputFocus(Args);
}

void CNativeWindowProxyBase::OnWindowInputBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowInputBlur(Args);
}

void CNativeWindowProxyBase::OnWindowMinimize(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMinimize(Args);
}

void CNativeWindowProxyBase::OnWindowRestore(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowRestore(Args);
}

void CNativeWindowProxyBase::OnWindowResize(ResizeEventArgs& Args)
{
	GetMyEventsListener()->OnWindowResize(Args);
}

void CNativeWindowProxyBase::OnWindowClose(WindowCloseEventArgs& Args)
{
	GetMyEventsListener()->OnWindowClose(Args);
}

bool CNativeWindowProxyBase::OnWindowKeyPressed(KeyEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowKeyPressed(Args);
}

void CNativeWindowProxyBase::OnWindowKeyReleased(KeyEventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyReleased(Args);
}

void CNativeWindowProxyBase::OnWindowKeyboardFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyboardFocus(Args);
}

void CNativeWindowProxyBase::OnWindowKeyboardBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyboardBlur(Args);
}

void CNativeWindowProxyBase::OnWindowMouseMoved(MouseMotionEventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseMoved(Args);
}

bool CNativeWindowProxyBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowMouseButtonPressed(Args);
}

void CNativeWindowProxyBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseButtonReleased(Args);
}

bool CNativeWindowProxyBase::OnWindowMouseWheel(MouseWheelEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowMouseWheel(Args);
}

void CNativeWindowProxyBase::OnWindowMouseLeave(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseLeave(Args);
}

void CNativeWindowProxyBase::OnWindowMouseFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseFocus(Args);
}

void CNativeWindowProxyBase::OnWindowMouseBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseBlur(Args);
}


//
// Protected
//
IznNativeWindowEventListener* CNativeWindowProxyBase::GetMyEventsListener() const
{
	if (m_EventListener == nullptr)
		throw CException("You must set event listener for NativeWindow.");
	return m_EventListener;
}

IznNativeWindow & CNativeWindowProxyBase::GetSourceNativeWindow() const
{
	return m_SourceNativeWindow;
}