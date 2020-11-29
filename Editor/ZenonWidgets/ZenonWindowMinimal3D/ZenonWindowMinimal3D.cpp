#include "stdafx.h"

// General
#include "ZenonWindowMinimal3D.h"

ZenonWindowMinimal3DWidget::ZenonWindowMinimal3DWidget(QWidget *parent)
	: QWidget(parent)

	, m_EventListener(nullptr)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_NoMousePropagation);
}

ZenonWindowMinimal3DWidget::~ZenonWindowMinimal3DWidget()
{
}



//
// IznNativeWindow
//
void ZenonWindowMinimal3DWidget::SetWindowTitle(const std::string& WindowName)
{
	throw CException("Not implemented");
}

std::string ZenonWindowMinimal3DWidget::GetWindowTitle() const
{
	return "ZenonWindowMinimal3DWidget";
}

size_t ZenonWindowMinimal3DWidget::GetWindowWidth() const
{
	return geometry().width();
}

size_t ZenonWindowMinimal3DWidget::GetWindowHeight() const
{
	return geometry().height();
}

void ZenonWindowMinimal3DWidget::SetCursorPosition(const glm::ivec2& CursorPosition)
{
	throw CException("Not implemented");
}

glm::ivec2 ZenonWindowMinimal3DWidget::GetCursorPosition() const
{
	throw CException("Not implemented");
}

void ZenonWindowMinimal3DWidget::ShowCursor()
{
	throw CException("Not implemented");
}

void ZenonWindowMinimal3DWidget::HideCursor()
{
	throw CException("Not implemented");
}

void ZenonWindowMinimal3DWidget::Close()
{
	// Do nothing
}

void ZenonWindowMinimal3DWidget::SetEventsListener(IznNativeWindowEventListener * WindowEventsListener)
{
	m_EventListener = WindowEventsListener;
}

void ZenonWindowMinimal3DWidget::ResetEventsListener()
{
	m_EventListener = nullptr;
}



//
// IznNativeWindow_WindowsSpecific
//
HWND ZenonWindowMinimal3DWidget::GetHWnd() const
{
	return (HWND)winId();
}

std::shared_ptr<IImage> ZenonWindowMinimal3DWidget::TakeScreenshot(IBaseManager& BaseManager)
{



	{
		RECT rc;
		HWND hwnd = GetParent(GetParent(GetHWnd()));    //the window can't be min
		if (hwnd == NULL)
		{
			//std::cout << "it can't find any 'note' window" << std::endl;
			//return 0;
		}
		GetClientRect(hwnd, &rc);

		//create
		HDC hdcScreen = GetDC(NULL);
		HDC hdc = CreateCompatibleDC(hdcScreen);
		HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);
		SelectObject(hdc, hbmp);

		//Print to memory hdc
		PrintWindow(hwnd, hdc, PW_CLIENTONLY);

		//copy to clipboard
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, hbmp);
		CloseClipboard();

		//release
		DeleteDC(hdc);
		DeleteObject(hbmp);
		ReleaseDC(NULL, hdcScreen);

		//std::cout << "success copy to clipboard, please paste it to the 'mspaint'" << std::endl;
	}



	HWND hwnd = GetParent(GetParent(GetHWnd()));

	HDC hdcScreen = GetDC(hwnd);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, GetWindowWidth(), GetWindowHeight());
	SelectObject(hdc, hBitmap);

	if (FALSE == PrintWindow(hwnd, hdc, PW_CLIENTONLY))
		_ASSERT(false);

	//HBITMAP prevBitmap = (HBITMAP)SelectObject(hdc, hBitmap);

	BITMAP bitmap = { };
	if (false == GetObjectA(hBitmap, sizeof(BITMAP), &bitmap))
		_ASSERT(false);

	BITMAPINFO bitmapInfo = { };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	if (false == GetDIBits(hdc, hBitmap, 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS))
		_ASSERT(false);

	bitmapInfo.bmiHeader.biBitCount = 32;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	std::shared_ptr<IImage> image = BaseManager.GetManager<IImagesFactory>()->CreateEmptyImage("screenshoot.png", GetWindowWidth(), GetWindowHeight(), 32);
	uint8* imageDataEx = image->GetDataEx();

	for (LONG i = 0; i < bitmap.bmHeight; i++)
		if (false == GetDIBits(hdc, hBitmap, i, 1, &imageDataEx[(bitmap.bmHeight - 1 - i) * GetWindowWidth() * 4], &bitmapInfo, DIB_RGB_COLORS))
			_ASSERT(false);

	//SelectObject(hdc, prevBitmap);

	DeleteDC(hdc);
	DeleteObject(hBitmap);
	ReleaseDC(hwnd, hdcScreen);

	return image;
}


//
// Events
//

void ZenonWindowMinimal3DWidget::focusInEvent(QFocusEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowInputFocus(EventArgs());
}

void ZenonWindowMinimal3DWidget::focusOutEvent(QFocusEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowInputBlur(EventArgs());
}

void ZenonWindowMinimal3DWidget::enterEvent(QEvent * event)
{
	setFocus();

	if (m_EventListener)
		m_EventListener->OnWindowMouseFocus(EventArgs());
}

void ZenonWindowMinimal3DWidget::leaveEvent(QEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowMouseLeave(EventArgs());
}

void ZenonWindowMinimal3DWidget::paintEvent(QPaintEvent *pEvent)
{
	__super::paintEvent(pEvent);
}

void ZenonWindowMinimal3DWidget::moveEvent(QMoveEvent * event)
{
	// do nothing
}

void ZenonWindowMinimal3DWidget::resizeEvent(QResizeEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowResize(ResizeEventArgs(event->size().width(), event->size().height()));
}

void ZenonWindowMinimal3DWidget::closeEvent(QCloseEvent * event)
{
	if (m_EventListener)
		m_EventListener->OnWindowClose(WindowCloseEventArgs());
}

void ZenonWindowMinimal3DWidget::showEvent(QShowEvent * event)
{
}

void ZenonWindowMinimal3DWidget::hideEvent(QHideEvent * event)
{
	//PostQuitMessage(0);
}
