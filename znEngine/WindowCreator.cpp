#include "stdafx.h"

// General
#include "WindowCreator.h"



// STATIC BEGIN

LRESULT CALLBACK WndProcLink(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWindowObject* windowObject = (CWindowObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (windowObject != nullptr)
	{
		return windowObject->WndProc(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

MouseButtonEventArgs::MouseButton DecodeMouseButton(UINT messageID)
{
	switch (messageID)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
		return MouseButtonEventArgs::MouseButton::Left;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return MouseButtonEventArgs::MouseButton::Right;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
		return MouseButtonEventArgs::MouseButton::Middel;
	}

	return MouseButtonEventArgs::MouseButton::None;
}

// STATIC END



LRESULT CWindowObject::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_CREATE:
		return 0;

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

		m_KeyPressed(KeyEventArgs(this, key, c, KeyEventArgs::Pressed, control, shift, alt));
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
			_ASSERT_EXPR(false, "CWindowObject: Failed to '::GetKeyboardState'.");

		wchar_t translatedCharacters[4];
		if (int result = ::ToUnicodeEx((UINT)wParam, scanCode, keyboardState, translatedCharacters, 4, 0, NULL) > 0)
		{
			c = translatedCharacters[0];
		}

		m_KeyReleased(KeyEventArgs(this, key, c, KeyEventArgs::Released, control, shift, alt));
	}
	break;
	case WM_SETFOCUS:
	{
		m_bHasKeyboardFocus = true;

		m_KeyboardFocus(EventArgs(this));
	}
	break;
	case WM_KILLFOCUS:
	{
		m_bHasKeyboardFocus = false;

		m_KeyboardBlur(EventArgs(this));
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

		if (!m_InClientRect)
		{
			m_PreviousMousePosition = glm::ivec2(x, y);
			m_InClientRect = true;
		}

		MouseMotionEventArgs mouseMotionEventArgs(this, lButton, mButton, rButton, control, shift, x, y);
		mouseMotionEventArgs.RelX = mouseMotionEventArgs.X - m_PreviousMousePosition.x;
		mouseMotionEventArgs.RelY = mouseMotionEventArgs.Y - m_PreviousMousePosition.y;

		m_PreviousMousePosition = glm::ivec2(mouseMotionEventArgs.X, mouseMotionEventArgs.Y);

		m_MouseMoved(mouseMotionEventArgs);
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

		m_MouseButtonPressed(MouseButtonEventArgs(this, DecodeMouseButton(message), MouseButtonEventArgs::ButtonState::Pressed, lButton, mButton, rButton, control, shift, x, y));
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

		m_MouseButtonReleased(MouseButtonEventArgs(this, DecodeMouseButton(message), MouseButtonEventArgs::ButtonState::Released, lButton, mButton, rButton, control, shift, x, y));
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
			_ASSERT_EXPR(false, "CWindowObject: Failed to '::ScreenToClient'.");

		m_MouseWheel(MouseWheelEventArgs(this, zDelta, lButton, mButton, rButton, control, shift, (int)clientToScreenPoint.x, (int)clientToScreenPoint.y));
	}
	break;
	case WM_MOUSELEAVE: // Сообщение WM_MOUSELEAVE посылается в окно тогда, когда курсор оставляет рабочую область окна, заданную при предшествующем вызове функции TrackMouseEvent.
	{
		m_IsMouseTracking = false;
		m_InClientRect = false;

		m_MouseLeave(EventArgs(this));
	}
	break;
	case WM_MOUSEACTIVATE: // Сообщение WM_MOUSEACTIVATE отправляется тогда, когда курсор находится в неактивном окне, а пользователь нажимает кнопку мыши.
	{
		m_MouseFocus(EventArgs(this));
	}
	break;
	case WM_CAPTURECHANGED: // Sent to the window that is losing the mouse capture.
	{
		m_MouseBlur(EventArgs(this));
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
			m_InputFocus(EventArgs(this));
		}
		break;

		case WA_INACTIVE:
		{
			m_InputBlur(EventArgs(this));
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
			m_Minimize(EventArgs(this));
		}
		break;

		case SIZE_MAXIMIZED:
		{
			m_Restore(EventArgs(this));
		}
		break;

		case SIZE_RESTORED:
		{
			int width = ((int)(short)LOWORD(lParam));
			int height = ((int)(short)HIWORD(lParam));

			if (width != 0 && height != 0)
			{
				m_Resize(ResizeEventArgs(this, width, height));
			}
		}
		break;
		}

		::UpdateWindow(hwnd);
	}
	break;


	case WM_CLOSE:
	{
		Log::Warn("WndProc: WM_CLOSE.");

		WindowCloseEventArgs windowCloseEventArgs(this);

		m_Close(windowCloseEventArgs);

		if (windowCloseEventArgs.ConfirmClose)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
	break;



	case WM_DESTROY:
	{
		Log::Warn("WndProc: WM_DESTROY.");
	}
	break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}




CWindowObject::CWindowObject()
	: m_HInstance(NULL)
	, m_HWnd(NULL)

	, m_PreviousMousePosition(0, 0)
	, m_InClientRect(false)
	, m_IsMouseTracking(false)

	, m_bHasKeyboardFocus(false)
{}

CWindowObject::~CWindowObject()
{
	DestroyWindowInstance();
}



//
// IWindowObject
//
std::string CWindowObject::GetWindowName()
{
	std::wstring title(GetWindowTextLength(m_HWnd) + 1, L'\0');
	GetWindowText(m_HWnd, &title[0], title.size());
	return Resources::ConvertString(title);
}

long CWindowObject::GetWindowWidth()
{
	RECT clientRect = { 0 };

    if (::GetClientRect(m_HWnd, &clientRect) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::GetClientRect'.");

	return clientRect.right - clientRect.left;
}

long CWindowObject::GetWindowHeight()
{
	RECT clientRect = { 0 };

    if (::GetClientRect(m_HWnd, &clientRect) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::GetClientRect'.");

	return clientRect.bottom - clientRect.top;
}

HWND CWindowObject::GetHWnd()
{
	return m_HWnd;
}

void CWindowObject::SetCursorPosition(const glm::ivec2& CursorPosition)
{
	RECT rect = { 0 };

	if (::GetClientRect(m_HWnd, &rect) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::GetClientRect'.");

	if (::ClientToScreen(m_HWnd, reinterpret_cast<POINT*>(&rect.left)) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::ClientToScreen'.");

	if (::ClientToScreen(m_HWnd, reinterpret_cast<POINT*>(&rect.right)) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::ClientToScreen'.");

	if (::SetCursorPos(rect.left + CursorPosition.x, rect.top + CursorPosition.y) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::SetCursorPos'.");
}

glm::ivec2 CWindowObject::GetCursorPosition() const
{
	POINT point = { 0 };

	if (::GetCursorPos(&point) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::GetCursorPos'.");

	if (::ScreenToClient(m_HWnd, &point) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to '::ScreenToClient'.");

	return glm::ivec2(point.x, point.y);
}

void CWindowObject::ShowCursor()
{
	::ShowCursor(TRUE);
}

void CWindowObject::HideCursor()
{
	::ShowCursor(FALSE);
}





//
// IWindowEvents
//

// Window events
Event& CWindowObject::InputFocus()
{
	return m_InputFocus;
}

Event& CWindowObject::InputBlur()
{
	return m_InputBlur;
}

Event& CWindowObject::Minimize()
{
	return m_Minimize;
}

Event& CWindowObject::Restore()
{
	return m_Restore;
}

ResizeEvent& CWindowObject::Resize()
{
	return m_Resize;
}

WindowCloseEvent& CWindowObject::Close()
{
	return m_Close;
}



// Keyboard events
KeyboardEvent& CWindowObject::KeyPressed()
{
	return m_KeyPressed;
}

KeyboardEvent& CWindowObject::KeyReleased()
{
	return m_KeyReleased;
}

Event& CWindowObject::KeyboardFocus()
{
	return m_KeyboardFocus;
}

Event& CWindowObject::KeyboardBlur()
{
	return m_KeyboardBlur;
}


// Mouse events
MouseMotionEvent& CWindowObject::MouseMoved()
{
	return m_MouseMoved;
}

MouseButtonEvent& CWindowObject::MouseButtonPressed()
{
	return m_MouseButtonPressed;
}

MouseButtonEvent& CWindowObject::MouseButtonReleased()
{
	return m_MouseButtonReleased;
}

MouseWheelEvent& CWindowObject::MouseWheel()
{
	return m_MouseWheel;
}

Event& CWindowObject::MouseLeave()
{
	return m_MouseLeave;
}

Event& CWindowObject::MouseFocus()
{
	return m_MouseFocus;
}

Event& CWindowObject::MouseBlur()
{
	return m_MouseBlur;
}



//
// IWindowObjectEx
//
void CWindowObject::SetWindowHandle(HWND HWnd)
{
	_ASSERT(m_HWnd == NULL);
	m_HWnd = HWnd;
}

HWND CWindowObject::CreateWindowInstance(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height)
{
	_ASSERT_EXPR(m_HWnd == NULL, "Window already created.");

	//
	// Register window class
	//

	m_HInstance = Application->GetHINSTANCE();
	m_WindowClassName = std::wstring(WindowName) + std::wstring(L"Сlass");

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	WNDCLASSEX renderWindowClass = { 0 };
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &WndProcLink;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = m_HInstance;
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = m_WindowClassName.c_str();
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (::RegisterClassEx(&renderWindowClass) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to register the render window class.");

	//
	// Create window
	//

	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, Width, Height };
	if (::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to AdjustWindowRect.");

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	m_HWnd = CreateWindowEx
	(
		NULL,
		m_WindowClassName.c_str(),
		WindowName,
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		m_HInstance,
		NULL
	);

	if (m_HWnd == NULL)
		_ASSERT_EXPR(false, L"CWindowObject: Failed to create render window.");

	SetWindowLongPtr(m_HWnd, GWLP_USERDATA, (LONG_PTR)dynamic_cast<CWindowObject*>(this));

	if (::ShowWindow(m_HWnd, SW_SHOWDEFAULT) == FALSE)
	{
	//	_ASSERT_EXPR(false, L"CWindowObject: Failed to show window.");
	}

	if (::BringWindowToTop(m_HWnd))
	{
	//	_ASSERT_EXPR(false, "CWindowObject: Failed to bring window to top.");
	}

    return m_HWnd;
}

void CWindowObject::DestroyWindowInstance()
{
	_ASSERT(m_HWnd != NULL);

	if (::DestroyWindow(m_HWnd) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to destroy window object.");

	if (::UnregisterClass(m_WindowClassName.c_str(), m_HInstance) == FALSE)
		_ASSERT_EXPR(false, "CWindowObject: Failed to unregister render window class.");
}