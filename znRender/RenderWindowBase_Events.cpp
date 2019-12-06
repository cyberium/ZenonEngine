#include "stdafx.h"

// General
#include "RenderWindowBase.h"

MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID)
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

LRESULT RenderWindowBase::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		KeyEventArgs keyEventArgs(this, key, c, KeyEventArgs::Pressed, control, shift, alt);
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

		KeyEventArgs keyEventArgs(this, key, c, KeyEventArgs::Released, control, shift, alt);
		OnKeyReleased(keyEventArgs);
	}
	break;
	case WM_SETFOCUS:
	{
		EventArgs eventArgs(this);
		OnKeyboardFocus(eventArgs);
	}
	break;
	case WM_KILLFOCUS:
	{
		// Window lost keyboard focus.
		EventArgs eventArgs(this);
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

		MouseMotionEventArgs mouseMotionEventArgs(this, lButton, mButton, rButton, control, shift, x, y);
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

		MouseButtonEventArgs mouseButtonEventArgs(this, DecodeMouseButton(message), MouseButtonEventArgs::Pressed, lButton, mButton, rButton, control, shift, x, y);
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

		MouseButtonEventArgs mouseButtonEventArgs(this, DecodeMouseButton(message), MouseButtonEventArgs::Released, lButton, mButton, rButton, control, shift, x, y);
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

		MouseWheelEventArgs mouseWheelEventArgs(this, zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y);
		OnMouseWheel(mouseWheelEventArgs);
	}
	break;
	case WM_MOUSELEAVE:
	{
		EventArgs mouseLeaveEventArgs(this);
		OnMouseLeave(mouseLeaveEventArgs);
	}
	break;
	case WM_MOUSEACTIVATE:
	{
		EventArgs mouseFocusEventArgs(this);
		OnMouseFocus(mouseFocusEventArgs);
	}
	break;
	// NOTE: Not really sure if these next set of messages are working correctly.
	// Not really sure HOW to get them to work correctly.
	// TODO: Try to fix these if I need them ;)
	case WM_CAPTURECHANGED:
	{
		EventArgs mouseBlurEventArgs(this);
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
			EventArgs inputFocusEventArgs(this);
			OnInputFocus(inputFocusEventArgs);
		}
		break;

		case WA_INACTIVE:
		{
			EventArgs inputBlueEventArgs(this);
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
			EventArgs mininizeEventArgs(this);
			OnMinimize(mininizeEventArgs);
		}
		break;

		case SIZE_MAXIMIZED:
		{
			EventArgs restoreEventArgs(this);
			OnRestore(restoreEventArgs);
		}
		break;

		case SIZE_RESTORED:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			if (width != 0 && height != 0)
			{
				ResizeEventArgs resizeEventArgs(this, width, height);
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
		WindowCloseEventArgs windowCloseEventArgs(this);
		OnClose(windowCloseEventArgs);

		if (windowCloseEventArgs.ConfirmClose)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
	break;



	case WM_DESTROY:
	{
		Log::Warn("WndProc: Window destroyed.");
	}
	break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}



//
// IRenderWindowEvents
//
Event& RenderWindowBase::Initialize()
{
	return m_Initialize;
}
void RenderWindowBase::OnInitialize(EventArgs& e)
{
	EventArgs initializeEventArgs(this);
	m_Initialize(initializeEventArgs);
}

UpdateEvent& RenderWindowBase::Update()
{
	return m_Update;
}
void RenderWindowBase::OnUpdate(UpdateEventArgs& e)
{
	GetRenderDevice()->Lock();
	{
		RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, nullptr, nullptr, nullptr);
		OnPreRender(renderArgs);
		OnRender(renderArgs);
		OnPostRender(renderArgs);

		RenderEventArgs renderUIArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, nullptr, nullptr, nullptr);
		OnRenderUI(renderUIArgs);

		Present();
	}
	GetRenderDevice()->Unlock();

	UpdateEventArgs updateArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter);
	m_Update(updateArgs);
}

RenderEvent& RenderWindowBase::PreRender()
{
	return m_PreRender;
}
void RenderWindowBase::OnPreRender(RenderEventArgs& e)
{
	if (m_bResizePending)
	{
		ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
		m_bResizePending = false;
	}

	m_RenderTarget->Bind();

	RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_PreRender(renderArgs);
}

RenderEvent& RenderWindowBase::Render()
{
	return m_Render;
}
void RenderWindowBase::OnRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_Render(renderArgs);
}

RenderEvent& RenderWindowBase::PostRender()
{
	return m_PostRender;
}
void RenderWindowBase::OnPostRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_PostRender(renderArgs);

	//m_RenderTarget->UnBind();
}

RenderEvent& RenderWindowBase::RenderUI()
{
	return m_RenderUI;
}
void RenderWindowBase::OnRenderUI(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_RenderUI(renderArgs);
}

Event& RenderWindowBase::Terminate()
{
	return m_Terminate;
}
void RenderWindowBase::OnTerminate(EventArgs& e)
{
	e.Caller = this;
	m_Terminate(e);
}


//
// Window events
//
Event& RenderWindowBase::InputFocus()
{
	return m_InputFocus;
}
void RenderWindowBase::OnInputFocus(EventArgs& e) // The window has received focus
{
	e.Caller = this;
	m_InputFocus(e);
}

Event& RenderWindowBase::InputBlur()
{
	return m_InputBlur;
}
void RenderWindowBase::OnInputBlur(EventArgs& e) // The RenderWindowBase window has lost focus
{
	e.Caller = this;
	m_InputBlur(e);
}

Event& RenderWindowBase::Minimize()
{
	return m_Minimize;
}
void RenderWindowBase::OnMinimize(EventArgs& e) // The RenderWindowBase window has been minimized
{
	e.Caller = this;
	m_Minimize(e);
}

Event& RenderWindowBase::Restore()
{
	return m_Restore;
}
void RenderWindowBase::OnRestore(EventArgs& e) // The RenderWindowBase window has been restored
{
	m_bResizePending = true;

	e.Caller = this;
	m_Restore(e);
}

ResizeEvent& RenderWindowBase::Resize()
{
	return m_Resize;
}
void RenderWindowBase::OnResize(ResizeEventArgs& e) // The RenderWindowBase window has be resized
{
	m_Viewport.SetWidth(e.Width);
	m_Viewport.SetHeight(e.Height);

	m_bResizePending = true;

	e.Caller = this;
	m_Resize(e);
}

Event& RenderWindowBase::Expose()
{
	return m_Expose;
}
void RenderWindowBase::OnExpose(EventArgs& e) // The window contents should be repainted
{
	e.Caller = this;
	m_Expose(e);
}

WindowCloseEvent& RenderWindowBase::Close()
{
	return m_Close;
}
void RenderWindowBase::OnClose(WindowCloseEventArgs& e)
{
	e.Caller = this;
	m_Close(e);
}



//
// Keyboard events
//
KeyboardEvent& RenderWindowBase::KeyPressed()
{
	return m_KeyPressed;
}
void RenderWindowBase::OnKeyPressed(KeyEventArgs& e) // A keyboard key was pressed
{
	e.Caller = this;
	m_KeyPressed(e);
}

KeyboardEvent& RenderWindowBase::KeyReleased()
{
	return m_KeyReleased;
}
void RenderWindowBase::OnKeyReleased(KeyEventArgs& e) // A keyboard key was released
{
	e.Caller = this;
	m_KeyReleased(e);
}

Event& RenderWindowBase::KeyboardFocus()
{
	return m_KeyboardFocus;
}
void RenderWindowBase::OnKeyboardFocus(EventArgs& e) // Window gained keyboard focus
{
	m_bHasKeyboardFocus = true;

	e.Caller = this;
	m_KeyboardFocus(e);
}

Event& RenderWindowBase::KeyboardBlur()
{
	return m_KeyboardBlur;
}
void RenderWindowBase::OnKeyboardBlur(EventArgs& e) // Window lost keyboard focus
{
	m_bHasKeyboardFocus = false;

	e.Caller = this;
	m_KeyboardBlur(e);
}


//
// The mouse events
//
MouseMotionEvent& RenderWindowBase::MouseMoved()
{
	return m_MouseMoved;
}
void RenderWindowBase::OnMouseMoved(MouseMotionEventArgs& e)
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

	e.Caller = this;
	m_MouseMoved(e);
}

MouseButtonEvent& RenderWindowBase::MouseButtonPressed()
{
	return m_MouseButtonPressed;
}
void RenderWindowBase::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	e.Caller = this;
	m_MouseButtonPressed(e);
}

MouseButtonEvent& RenderWindowBase::MouseButtonReleased()
{
	return m_MouseButtonReleased;
}
void RenderWindowBase::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	e.Caller = this;
	m_MouseButtonReleased(e);
}

MouseWheelEvent& RenderWindowBase::MouseWheel()
{
	return m_MouseWheel;
}
void RenderWindowBase::OnMouseWheel(MouseWheelEventArgs& e)
{
	e.Caller = this;
	m_MouseWheel(e);
}

Event& RenderWindowBase::MouseLeave()
{
	return m_MouseLeave;
}
void RenderWindowBase::OnMouseLeave(EventArgs& e)
{
	m_IsMouseTracking = false;
	m_InClientRect = false;

	e.Caller = this;
	m_MouseLeave(e);
}

Event& RenderWindowBase::MouseFocus()
{
	return m_MouseFocus;
}
void RenderWindowBase::OnMouseFocus(EventArgs& e) // The window has received mouse focus
{
	e.Caller = this;
	m_MouseFocus(e);
}

Event& RenderWindowBase::MouseBlur()
{
	return m_MouseBlur;
}
void RenderWindowBase::OnMouseBlur(EventArgs& e) // The window has lost mouse focus
{
	e.Caller = this;
	m_MouseBlur(e);
}