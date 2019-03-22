#include "stdafx.h"

// General
#include "RenderWindow.h"

// Additional
#include "Camera.h"

RenderWindow::RenderWindow(IWindowObject * WindowObject, bool vSync)
	: m_WindowObject(WindowObject)
	, m_vSync(vSync)

	, m_PreviousMousePosition(0, 0)
	, m_bInClientRect(false)
	, m_bIsMouseTracking(false)

	, m_bHasKeyboardFocus(false)
{}

RenderWindow::~RenderWindow()
{}



void RenderWindow::ShowWindow()
{
	m_WindowObject->ShowWindow(SW_SHOWDEFAULT);
	m_WindowObject->BringWindowToTop();
}

void RenderWindow::HideWindow()
{
	m_WindowObject->ShowWindow(SW_HIDE);
}

void RenderWindow::CloseWindow()
{
	m_WindowObject->DestroyWindow();
}

void RenderWindow::SetMousePosition(vec2 _position)
{
	RECT rc;
	m_WindowObject->GetClientRect(&rc);
	m_WindowObject->ClientToScreen(reinterpret_cast<POINT*>(&rc.left));
	m_WindowObject->ClientToScreen(reinterpret_cast<POINT*>(&rc.right));

	::SetCursorPos(rc.left + _position.x, rc.top + _position.y);
}


void RenderWindow::SetWindowName(cstring _name)
{
	//m_sWindowName = _name;

	//::SetWindowTextA(m_HWND, m_sWindowName.c_str());
}

std::string RenderWindow::GetWindowName() const
{
	return m_WindowObject->GetWindowName();
}

int RenderWindow::GetWindowWidth() const
{
    RECT clientRect;
    m_WindowObject->GetClientRect(&clientRect);

	return clientRect.right - clientRect.left;
}

int RenderWindow::GetWindowHeight() const
{
    RECT clientRect;
    m_WindowObject->GetClientRect(&clientRect);

    return clientRect.bottom - clientRect.top;
}

glm::ivec2 RenderWindow::GetWindowSize() const
{
	return glm::ivec2(GetWindowWidth(), GetWindowHeight());
}

bool RenderWindow::IsVSync() const
{
	return m_vSync;
}

HWND RenderWindow::GetHWnd() const
{
	return m_WindowObject->GetHWnd();
}



//
// Engine events
//
void RenderWindow::OnInitialize(EventArgs& e)
{
	Initialize(e);
}

void RenderWindow::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void RenderWindow::OnPreRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PreRender(renderArgs);
}

void RenderWindow::OnRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	Render(renderArgs);
}

void RenderWindow::OnPostRender(Render3DEventArgs& e)
{
	Render3DEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState);
	PostRender(renderArgs);
}

void RenderWindow::OnRenderUI(RenderUIEventArgs& e)
{
	RenderUIEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter);
	RenderUI(renderArgs);
}

void RenderWindow::OnTerminate(EventArgs& e)
{
	Terminate(e);
}



//
// Window events
//
void RenderWindow::OnInputFocus(EventArgs& e) // The window has received focus
{
	InputFocus(e);
}

void RenderWindow::OnInputBlur(EventArgs& e) // The RenderWindow window has lost focus
{
	InputBlur(e);
}

void RenderWindow::OnMinimize(EventArgs& e) // The RenderWindow window has been minimized
{
	Minimize(e);
}

void RenderWindow::OnRestore(EventArgs& e) // The RenderWindow window has been restored
{
	Restore(e);
}

void RenderWindow::OnResize(ResizeEventArgs& e) // The RenderWindow window has be resized
{
	// TODO
	//m_iWindowWidth = e.Width;
	//m_iWindowHeight = e.Height;

	Resize(e);

	HideWindow();
	ShowWindow();
}

void RenderWindow::OnExpose(EventArgs& e) // The window contents should be repainted
{
	Expose(e);
}

void RenderWindow::OnClose(WindowCloseEventArgs& e)
{
	Close(e);
}



//
// Keyboard events
//
void RenderWindow::OnKeyPressed(KeyEventArgs& e) // A keyboard key was pressed
{
	KeyPressed(e);
}

void RenderWindow::OnKeyReleased(KeyEventArgs& e) // A keyboard key was released
{
	KeyReleased(e);
}

void RenderWindow::OnKeyboardFocus(EventArgs& e) // Window gained keyboard focus
{
	m_bHasKeyboardFocus = true;
	KeyboardFocus(e);
}

void RenderWindow::OnKeyboardBlur(EventArgs& e) // Window lost keyboard focus
{
	m_bHasKeyboardFocus = false;
	KeyboardBlur(e);
}


//
// The mouse events
//

void RenderWindow::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (!m_bIsMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_WindowObject->GetHWnd();
		if (::TrackMouseEvent(&tme))
		{
			m_bIsMouseTracking = true;
		}
	}

	if (!m_bInClientRect)
	{
		m_PreviousMousePosition = glm::ivec2(e.X, e.Y);
		m_bInClientRect = true;
	}

	e.RelX = e.X - m_PreviousMousePosition.x;
	e.RelY = e.Y - m_PreviousMousePosition.y;

	m_PreviousMousePosition = glm::ivec2(e.X, e.Y);

	MouseMoved(e);
}

void RenderWindow::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	MouseButtonPressed(e);
}

void RenderWindow::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	MouseButtonReleased(e);
}

void RenderWindow::OnMouseWheel(MouseWheelEventArgs& e)
{
	MouseWheel(e);
}

void RenderWindow::OnMouseLeave(EventArgs& e)
{
	m_bIsMouseTracking = false;
	m_bInClientRect = false;

	MouseLeave(e);
}

void RenderWindow::OnMouseFocus(EventArgs& e) // The window has received mouse focus
{
	MouseFocus(e);
}

void RenderWindow::OnMouseBlur(EventArgs& e) // The window has lost mouse focus
{
	MouseBlur(e);
}

