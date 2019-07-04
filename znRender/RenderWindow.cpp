#include "stdafx.h"

// Include
#include "RenderDevice.h"

// General
#include "RenderWindow.h"

// Additional
#include "Camera.h"

RenderWindow::RenderWindow(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync)
	: m_Device(RenderDevice)
    , m_WindowObject(WindowObject)
	, m_vSync(vSync)

    , m_bResizePending(false)

	, m_PreviousMousePosition(0, 0)
	, m_InClientRect(false)
	, m_IsMouseTracking(false)

	, m_bHasKeyboardFocus(false)
{

}

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


void RenderWindow::SetWindowName(const std::string& _name)
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

std::shared_ptr<IRenderDevice> RenderWindow::GetRenderDevice() const
{
    return m_Device.lock();
}

std::shared_ptr<IRenderTarget> RenderWindow::GetRenderTarget() const
{
    return m_RenderTarget;
}

void RenderWindow::CreateSwapChain()
{
    m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
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

void RenderWindow::OnPreRender(RenderEventArgs& e)
{
    if (m_bResizePending)
    {
        ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
        m_bResizePending = false;
    }

    m_RenderTarget->Bind();

	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node, e.Viewport);
	PreRender(renderArgs);
}

void RenderWindow::OnRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node, e.Viewport);
	Render(renderArgs);
}

void RenderWindow::OnPostRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node, e.Viewport);
	PostRender(renderArgs);

    //m_RenderTarget->UnBind();
}

void RenderWindow::OnRenderUI(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node, e.Viewport);
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
	Resize(e);

	HideWindow();
	ShowWindow();

    m_bResizePending = true;
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
	if (!m_IsMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_WindowObject->GetHWnd();
		if (::TrackMouseEvent(&tme))
		{
			m_IsMouseTracking = true;
		}
	}

	if (!m_InClientRect)
	{
		m_PreviousMousePosition = glm::ivec2(e.X, e.Y);
		m_InClientRect = true;
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
	m_IsMouseTracking = false;
	m_InClientRect = false;

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

