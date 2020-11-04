#include "stdafx.h"

// General
#include "NativeWindow_Proxy.h"


CNativeWindow_Proxy::CNativeWindow_Proxy(IznNativeWindow& SourceNativeWindow)
	: m_SourceNativeWindow(SourceNativeWindow)
	, m_EventListener(nullptr)
{
	m_SourceNativeWindow.SetEventsListener(this);
}

CNativeWindow_Proxy::~CNativeWindow_Proxy()
{
	m_SourceNativeWindow.ResetEventsListener();
}



//
// IznNativeWindow
//
void CNativeWindow_Proxy::SetWindowTitle(const std::string& WindowName)
{
	m_SourceNativeWindow.SetWindowTitle(WindowName);
}

std::string CNativeWindow_Proxy::GetWindowTitle() const
{
	return m_SourceNativeWindow.GetWindowTitle();
}

size_t CNativeWindow_Proxy::GetWindowWidth() const
{
	return m_SourceNativeWindow.GetWindowWidth();
}

size_t CNativeWindow_Proxy::GetWindowHeight() const
{
	return m_SourceNativeWindow.GetWindowHeight();
}

void CNativeWindow_Proxy::SetCursorPosition(const glm::ivec2& CursorPosition)
{
	m_SourceNativeWindow.SetCursorPosition(CursorPosition);
}

glm::ivec2 CNativeWindow_Proxy::GetCursorPosition() const
{
	return m_SourceNativeWindow.GetCursorPosition();
}

void CNativeWindow_Proxy::ShowCursor()
{
	m_SourceNativeWindow.ShowCursor();
}

void CNativeWindow_Proxy::HideCursor()
{
	m_SourceNativeWindow.HideCursor();
}

void CNativeWindow_Proxy::Close()
{
	m_SourceNativeWindow.Close();
}

void CNativeWindow_Proxy::SetEventsListener(IznNativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void CNativeWindow_Proxy::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// IznNativeWindow_WindowsSpecific
//
HWND CNativeWindow_Proxy::GetHWnd() const
{
	return dynamic_cast<IznNativeWindow_WindowsSpecific&>(m_SourceNativeWindow).GetHWnd();
}



//
// IznNativeWindowEventListener
//
void CNativeWindow_Proxy::OnWindowInputFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowInputFocus(Args);
}

void CNativeWindow_Proxy::OnWindowInputBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowInputBlur(Args);
}

void CNativeWindow_Proxy::OnWindowMinimize(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMinimize(Args);
}

void CNativeWindow_Proxy::OnWindowRestore(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowRestore(Args);
}

void CNativeWindow_Proxy::OnWindowResize(ResizeEventArgs& Args)
{
	GetMyEventsListener()->OnWindowResize(Args);
}

void CNativeWindow_Proxy::OnWindowClose(WindowCloseEventArgs& Args)
{
	GetMyEventsListener()->OnWindowClose(Args);
}

bool CNativeWindow_Proxy::OnWindowKeyPressed(KeyEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowKeyPressed(Args);
}

void CNativeWindow_Proxy::OnWindowKeyReleased(KeyEventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyReleased(Args);
}

void CNativeWindow_Proxy::OnWindowKeyboardFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyboardFocus(Args);
}

void CNativeWindow_Proxy::OnWindowKeyboardBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowKeyboardBlur(Args);
}

void CNativeWindow_Proxy::OnWindowMouseMoved(MouseMotionEventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseMoved(Args);
}

bool CNativeWindow_Proxy::OnWindowMouseButtonPressed(MouseButtonEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowMouseButtonPressed(Args);
}

void CNativeWindow_Proxy::OnWindowMouseButtonReleased(MouseButtonEventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseButtonReleased(Args);
}

bool CNativeWindow_Proxy::OnWindowMouseWheel(MouseWheelEventArgs & Args)
{
	return GetMyEventsListener()->OnWindowMouseWheel(Args);
}

void CNativeWindow_Proxy::OnWindowMouseLeave(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseLeave(Args);
}

void CNativeWindow_Proxy::OnWindowMouseFocus(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseFocus(Args);
}

void CNativeWindow_Proxy::OnWindowMouseBlur(EventArgs & Args)
{
	GetMyEventsListener()->OnWindowMouseBlur(Args);
}

IznNativeWindowEventListener* CNativeWindow_Proxy::GetMyEventsListener()
{
	if (m_EventListener == nullptr)
		throw CException("You must set event listener for NativeWindow.");
	return m_EventListener;
}
