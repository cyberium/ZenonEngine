#include "stdafx.h"

// General
#include "NativeWindow_PlatformWindows.h"

namespace
{
	MouseButton DecodeMouseButton(UINT messageID)
	{
		switch (messageID)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
			return MouseButton::Left;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return MouseButton::Right;

		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
			return MouseButton::Middel;
		}

		return MouseButton::None;
	}
}



CNativeWindow_PlatformWindows::CNativeWindow_PlatformWindows(HWND HWnd)
	: m_PreviousMousePosition(0, 0)
	, m_InClientRect(false)
	, m_IsMouseTracking(false)
	, m_bHasKeyboardFocus(false)

	, m_HWnd(HWnd)
	, m_EventListener(nullptr)
{}

CNativeWindow_PlatformWindows::~CNativeWindow_PlatformWindows()
{
	if (::IsWindow(m_HWnd))
	{
		/*if (*/::DestroyWindow(m_HWnd);/* == FALSE)*/
			//throw CException("CNativeWindow_PlatformWindows: Failed to destroy window object.");
	}
}



//
// IznNativeWindow
//
void CNativeWindow_PlatformWindows::SetWindowTitle(const std::string& WindowName)
{
	SetWindowTextW(m_HWnd, Resources::utf8_to_utf16(WindowName).c_str());
}

std::string CNativeWindow_PlatformWindows::GetWindowTitle() const
{
	std::wstring title(GetWindowTextLengthW(m_HWnd) + 1, L'\0');
	GetWindowTextW(m_HWnd, &title[0], title.size());
	return Resources::utf16_to_utf8(title);
}

size_t CNativeWindow_PlatformWindows::GetWindowWidth() const
{
	RECT clientRect = { 0 };

	if (::GetClientRect(m_HWnd, &clientRect) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::GetClientRect'.");

	return clientRect.right - clientRect.left;
}

size_t CNativeWindow_PlatformWindows::GetWindowHeight() const
{
	RECT clientRect = { 0 };

	if (::GetClientRect(m_HWnd, &clientRect) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::GetClientRect'.");

	return clientRect.bottom - clientRect.top;
}

void CNativeWindow_PlatformWindows::SetCursorPosition(const glm::ivec2& CursorPosition)
{
	RECT rect = { 0 };

	if (::GetClientRect(m_HWnd, &rect) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::GetClientRect'.");

	if (::ClientToScreen(m_HWnd, reinterpret_cast<POINT*>(&rect.left)) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::ClientToScreen'.");

	if (::ClientToScreen(m_HWnd, reinterpret_cast<POINT*>(&rect.right)) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::ClientToScreen'.");

	if (::SetCursorPos(rect.left + CursorPosition.x, rect.top + CursorPosition.y) == FALSE)
		throw CException("CNativeWindow_PlatformWindows: Failed to '::SetCursorPos'.");
}

glm::ivec2 CNativeWindow_PlatformWindows::GetCursorPosition() const
{
	POINT point = { 0 };

	if (FALSE == ::GetCursorPos(&point))
		throw CException("CNativeWindow_PlatformWindows: Failed to '::GetCursorPos'.");

	if (FALSE == ::ScreenToClient(m_HWnd, &point))
		throw CException("CNativeWindow_PlatformWindows: Failed to '::ScreenToClient'.");

	return glm::ivec2(point.x, point.y);
}

void CNativeWindow_PlatformWindows::ShowCursor()
{
	::ShowCursor(TRUE);
}

void CNativeWindow_PlatformWindows::HideCursor()
{
	::ShowCursor(FALSE);
}

void CNativeWindow_PlatformWindows::Close()
{
	if (::IsWindow(m_HWnd))
	{
		if (FALSE == ::CloseWindow(m_HWnd))
			throw CException("CNativeWindow_PlatformWindows: Failed to '::CloseWindow'.");
	}
}

void CNativeWindow_PlatformWindows::SetEventsListener(IznNativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void CNativeWindow_PlatformWindows::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// IznNativeWindow_WindowsSpecific
//
HWND CNativeWindow_PlatformWindows::GetHWnd() const
{
	return m_HWnd;
}



//
// INativeWindow_WindowsSpecificEx
//
LRESULT CNativeWindow_PlatformWindows::ProcessMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct = {};
			HDC hDC = ::BeginPaint(hwnd, &paintStruct);
			::EndPaint(hwnd, &paintStruct);
		}
		break;

		//
		// Keyboard events
		//
		case WM_KEYDOWN:
		{
			// Get the unicode character (UTF-16)
			unsigned int c = 0;

			// For printable characters, the next message will be WM_CHAR.
			// This message contains the character code we need to send the KeyPressed event.
			// Inspired by the SDL 1.2 implementation.
			MSG charMsg = {};
			if (PeekMessageW(&charMsg, hwnd, 0, 0, PM_NOREMOVE) && charMsg.message == WM_CHAR)
			{
				GetMessageW(&charMsg, hwnd, 0, 0);
				c = static_cast<unsigned int>(charMsg.wParam);
			}

			bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
			bool control = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
			bool alt = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			KeyCode key = (KeyCode)wParam;
			unsigned int scanCode = (lParam & 0x00FF0000) >> 16;

			if (m_EventListener)
				m_EventListener->OnWindowKeyPressed(KeyEventArgs(key, c, KeyState::Pressed, control, shift, alt));
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

			// Determine which key was released by converting the key code and the scan code to a printable character (if possible).
			// Inspired by the SDL 1.2 implementation.
			unsigned char keyboardState[256];
			if (::GetKeyboardState(keyboardState) == FALSE)
				throw CException("CNativeWindow_PlatformWindows: Failed to '::GetKeyboardState'.");

			wchar_t translatedCharacters[4];
			if (int result = ::ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
			{
				c = translatedCharacters[0];
			}

			if (m_EventListener)
				m_EventListener->OnWindowKeyReleased(KeyEventArgs(key, c, KeyState::Released, control, shift, alt));
		}
		break;
		case WM_SETFOCUS:
		{
			m_bHasKeyboardFocus = true;

			if (m_EventListener)
				m_EventListener->OnWindowKeyboardFocus(EventArgs());
		}
		break;
		case WM_KILLFOCUS:
		{
			m_bHasKeyboardFocus = false;

			if (m_EventListener)
				m_EventListener->OnWindowKeyboardBlur(EventArgs());
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

			// Track mouse
			if (!m_IsMouseTracking)
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = m_HWnd;

				if (::TrackMouseEvent(&tme))
				{
					m_IsMouseTracking = true;
				}
			}

			if (false == m_InClientRect)
			{
				m_PreviousMousePosition = glm::ivec2(x, y);
				m_InClientRect = true;
			}

			MouseMotionEventArgs mouseMotionEventArgs(lButton, mButton, rButton, control, shift, x, y);
			mouseMotionEventArgs.RelX = mouseMotionEventArgs.X - m_PreviousMousePosition.x;
			mouseMotionEventArgs.RelY = mouseMotionEventArgs.Y - m_PreviousMousePosition.y;

			m_PreviousMousePosition = glm::ivec2(mouseMotionEventArgs.X, mouseMotionEventArgs.Y);

			if (m_EventListener)
				m_EventListener->OnWindowMouseMoved(mouseMotionEventArgs);
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

			if (m_EventListener)
				m_EventListener->OnWindowMouseButtonPressed(MouseButtonEventArgs(DecodeMouseButton(message), ButtonState::Pressed, lButton, mButton, rButton, control, shift, false, x, y));
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

			if (m_EventListener)
				m_EventListener->OnWindowMouseButtonReleased(MouseButtonEventArgs(DecodeMouseButton(message), ButtonState::Released, lButton, mButton, rButton, control, shift, false, x, y));
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
			POINT clientToScreenPoint = { 0 };
			clientToScreenPoint.x = x;
			clientToScreenPoint.y = y;

			if (::ScreenToClient(hwnd, &clientToScreenPoint) == FALSE)
				throw CException("CNativeWindow_PlatformWindows: Failed to '::ScreenToClient'.");

			if (m_EventListener)
				m_EventListener->OnWindowMouseWheel(MouseWheelEventArgs(zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y));
		}
		break;
		case WM_MOUSELEAVE: // ��������� WM_MOUSELEAVE ���������� � ���� �����, ����� ������ ��������� ������� ������� ����, �������� ��� �������������� ������ ������� TrackMouseEvent.
		{
			m_IsMouseTracking = false;
			m_InClientRect = false;

			if (m_EventListener)
				m_EventListener->OnWindowMouseLeave(EventArgs());
		}
		break;
		case WM_MOUSEACTIVATE: // ��������� WM_MOUSEACTIVATE ������������ �����, ����� ������ ��������� � ���������� ����, � ������������ �������� ������ ����.
		{
			if (m_EventListener)
				m_EventListener->OnWindowMouseFocus(EventArgs());
		}
		break;
		case WM_CAPTURECHANGED: // Sent to the window that is losing the mouse capture.
		{
			if (m_EventListener)
				m_EventListener->OnWindowMouseBlur(EventArgs());
		}
		break;




		//
		// Window events
		//

		case WM_ACTIVATE:
		{
			switch (wParam)
			{
			case WA_ACTIVE: // Activated by some method other than a mouse click (for example, by a call to the SetActiveWindow function or by use of the keyboard interface to select the window).
			case WA_CLICKACTIVE: // Activated by a mouse click.
			{
				if (m_EventListener)
					m_EventListener->OnWindowInputFocus(EventArgs());
			}
			break;

			case WA_INACTIVE:
			{
				if (m_EventListener)
					m_EventListener->OnWindowInputBlur(EventArgs());
			}
			break;
			}
		}
		break;


		/*case WM_SIZING:
		{
			RECT * rect = (RECT*)lParam;

			int width = rect->right - rect->left;
			int height = rect->bottom - rect->top;

			if (width != 0 && height != 0)
			{
				m_Resize(ResizeEventArgs(this, width, height));
			}
		}
		break;*/


		case WM_SIZE:
		{
			switch (wParam)
			{
			case SIZE_MINIMIZED: // The window has been minimized.
			{
				if (m_EventListener)
					m_EventListener->OnWindowMinimize(EventArgs());
			}
			break;

			case SIZE_MAXIMIZED: // The window has been maximized.
			{
				if (m_EventListener)
					m_EventListener->OnWindowRestore(EventArgs());
			}
			break;

			case SIZE_RESTORED: // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
			{
				int width = ((int)(short)LOWORD(lParam));
				int height = ((int)(short)HIWORD(lParam));

				if (width != 0 && height != 0)
				{
					if (m_EventListener)
						m_EventListener->OnWindowResize(ResizeEventArgs(width, height));
				}
			}
			break;
			}

			if (::UpdateWindow(hwnd) == FALSE)
				throw CException("CNativeWindow_PlatformWindows: Failed to '::UpdateWindow'.");
		}
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
