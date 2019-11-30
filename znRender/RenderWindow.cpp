#include "stdafx.h"

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

const Viewport * RenderWindow::GetViewport() const
{
	return &m_Viewport;
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

	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	PreRender(renderArgs);
}

void RenderWindow::OnRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	Render(renderArgs);
}

void RenderWindow::OnPostRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	PostRender(renderArgs);

    //m_RenderTarget->UnBind();
}

void RenderWindow::OnRenderUI(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
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
	m_Viewport.SetWidth(e.Width);
	m_Viewport.SetHeight(e.Height);

	Resize(e);

	//HideWindow();
	//ShowWindow();

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


static MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID)
{
	MouseButtonEventArgs::MouseButton mouseButton = MouseButtonEventArgs::None;
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Left;
	}
	break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Right;
	}
	break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	{
		mouseButton = MouseButtonEventArgs::Middel;
	}
	break;
	}

	return mouseButton;
}

LRESULT CALLBACK RenderWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT paintStruct;
		HDC hDC = ::BeginPaint(hwnd, &paintStruct);
		::EndPaint(hwnd, &paintStruct);
	}
	break;



	//
	// Keyboard events
	//
	case WM_KEYDOWN:
	{
		MSG charMsg;

		// Get the unicode character (UTF-16)
		unsigned int c = 0;

		// For printable characters, the next message will be WM_CHAR.
		// This message contains the character code we need to send the KeyPressed event.
		// Inspired by the SDL 1.2 implementation.
		if (PeekMessage(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
		{
			GetMessage(&charMsg, hwnd, 0, 0);
			c = static_cast<unsigned int>(charMsg.wParam);
		}

		bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
		bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
		bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

		KeyCode key = (KeyCode)wParam;
		unsigned int scanCode = (lParam & 0x00FF0000) >> 16;
		KeyEventArgs keyEventArgs(*this, key, c, KeyEventArgs::Pressed, control, shift, alt);
		OnKeyPressed(keyEventArgs);
	}
	break;
	case WM_KEYUP:
	{
		bool shift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
		bool control = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
		bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

		KeyCode key = (KeyCode)wParam;
		unsigned int c = 0;
		unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

		// Determine which key was released by converting the key code and the scan code
		// to a printable character (if possible).
		// Inspired by the SDL 1.2 implementation.
		unsigned char keyboardState[256];
		::GetKeyboardState(keyboardState);
		wchar_t translatedCharacters[4];
		if (int result = ::ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
		{
			c = translatedCharacters[0];
		}

		KeyEventArgs keyEventArgs(*this, key, c, KeyEventArgs::Released, control, shift, alt);
		OnKeyReleased(keyEventArgs);
	}
	break;
	case WM_SETFOCUS:
	{
		EventArgs eventArgs(*this);
		OnKeyboardFocus(eventArgs);
	}
	break;
	case WM_KILLFOCUS:
	{
		// Window lost keyboard focus.
		EventArgs eventArgs(*this);
		OnKeyboardBlur(eventArgs);
	}
	break;



	//
	// The mouse events
	//
	case WM_MOUSEMOVE:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseMotionEventArgs mouseMotionEventArgs(*this, lButton, mButton, rButton, control, shift, x, y);
		OnMouseMoved(mouseMotionEventArgs);
	}
	break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseButtonEventArgs mouseButtonEventArgs(*this, DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
		OnMouseButtonPressed(mouseButtonEventArgs);
	}
	break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		bool lButton = (wParam & MK_LBUTTON) != 0;
		bool rButton = (wParam & MK_RBUTTON) != 0;
		bool mButton = (wParam & MK_MBUTTON) != 0;
		bool shift = (wParam & MK_SHIFT) != 0;
		bool control = (wParam & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		MouseButtonEventArgs mouseButtonEventArgs(*this, DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
		OnMouseButtonReleased(mouseButtonEventArgs);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		// The distance the mouse wheel is rotated.
		// A positive value indicates the wheel was rotated to the right.
		// A negative value indicates the wheel was rotated to the left.
		float zDelta = ((int)(short)HIWORD(wParam)) / (float)WHEEL_DELTA;
		short keyStates = (short)LOWORD(wParam);

		bool lButton = (keyStates & MK_LBUTTON) != 0;
		bool rButton = (keyStates & MK_RBUTTON) != 0;
		bool mButton = (keyStates & MK_MBUTTON) != 0;
		bool shift = (keyStates & MK_SHIFT) != 0;
		bool control = (keyStates & MK_CONTROL) != 0;

		int x = ((int)(short)LOWORD(lParam));
		int y = ((int)(short)HIWORD(lParam));

		// Convert the screen coordinates to client coordinates.
		POINT clientToScreenPoint;
		clientToScreenPoint.x = x;
		clientToScreenPoint.y = y;
		::ScreenToClient(hwnd, &clientToScreenPoint);

		MouseWheelEventArgs mouseWheelEventArgs(*this, zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
		OnMouseWheel(mouseWheelEventArgs);
	}
	break;
	case WM_MOUSELEAVE:
	{
		EventArgs mouseLeaveEventArgs(*this);
		OnMouseLeave(mouseLeaveEventArgs);
	}
	break;
	case WM_MOUSEACTIVATE:
	{
		EventArgs mouseFocusEventArgs(*this);
		OnMouseFocus(mouseFocusEventArgs);
	}
	break;
	// NOTE: Not really sure if these next set of messages are working correctly.
	// Not really sure HOW to get them to work correctly.
	// TODO: Try to fix these if I need them ;)
	case WM_CAPTURECHANGED:
	{
		EventArgs mouseBlurEventArgs(*this);
		OnMouseBlur(mouseBlurEventArgs);
	}
	break;




	//
	// Window events
	//

	case WM_ACTIVATE:
	{
		switch (wParam)
		{
		case WA_ACTIVE:
		{
			EventArgs inputFocusEventArgs(*this);
			OnInputFocus(inputFocusEventArgs);
		}
		break;

		case WA_INACTIVE:
		{
			EventArgs inputBlueEventArgs(*this);
			OnInputBlur(inputBlueEventArgs);
		}
		break;
		}
	}
	break;


	case WM_SIZE:
	{
		switch (wParam)
		{
		case SIZE_MINIMIZED:
		{
			EventArgs mininizeEventArgs(*this);
			OnMinimize(mininizeEventArgs);
		}
		break;

		case SIZE_MAXIMIZED:
		{
			EventArgs restoreEventArgs(*this);
			OnRestore(restoreEventArgs);
		}
		break;

		case SIZE_RESTORED:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			if (width != 0 && height != 0)
			{
				ResizeEventArgs resizeEventArgs(*this, width, height);
				OnResize(resizeEventArgs);
			}
		}
		break;
		}

		::UpdateWindow(hwnd);
	}
	break;


	case WM_CLOSE:
	{
		WindowCloseEventArgs windowCloseEventArgs(*this);
		OnClose(windowCloseEventArgs);

		if (windowCloseEventArgs.ConfirmClose)
		{
			// Just hide the window.
			// Windows will be destroyed when the application quits.
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
	break;



	case WM_DESTROY:
	{
		// TODO delete gs_WindowHandle;
	}
	break;
	}


	return DefWindowProc(hwnd, message, wParam, lParam);
}

