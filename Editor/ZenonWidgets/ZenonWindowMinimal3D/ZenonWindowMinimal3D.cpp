#include "stdafx.h"

// General
#include "ZenonWindowMinimal3D.h"

// Additional
#include "EditorUI/ContextMenuUtils.h"

ZenonWindowMinimal3DWidget::ZenonWindowMinimal3DWidget(QWidget *parent)
	: QWidget(parent)
	, m_EventListener(nullptr)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_NoMousePropagation);

	// Add context menu for scene node viewer
	m_ContextMenu = MakeShared(QMenu, this);
	m_ContextMenu->setTitle("Some context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
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

void ZenonWindowMinimal3DWidget::SetOnContexMenu(OnWindow3DContextMenuCallback Callback)
{
	m_OnContextMenu = Callback;
}

void ZenonWindowMinimal3DWidget::SetOnDragEnter(OnWindow3DDragEnterCallback Callback)
{
	m_OnDragEnterCallback = Callback;
}

void ZenonWindowMinimal3DWidget::SetOnDragMove(OnWindow3DDragMoveCallback Callback)
{
	m_OnDragMoveCallback = Callback;
}

void ZenonWindowMinimal3DWidget::SetOnDragDrop(OnWindow3DDragDropCallback Callback)
{
	m_OnDragDropCallback = Callback;
}

void ZenonWindowMinimal3DWidget::SetOnDragLeave(OnWindow3DDragLeaveCallback Callback)
{
	m_OnDragLeaveCallback = Callback;
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

void ZenonWindowMinimal3DWidget::dragEnterEvent(QDragEnterEvent * event)
{
	event->ignore();

	const QMimeData * mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragEnterCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		glm::vec2 cursorPosition = glm::vec2(event->pos().x(), event->pos().y());
		bool isCtrl = (event->keyboardModifiers() & Qt::KeyboardModifier::ControlModifier) != 0;
		if (false == m_OnDragEnterCallback(cursorPosition, isCtrl, buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Exception occurs in 'dragEnterEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Unknown exception occurs in 'dragEnterEvent'.");
	}
}

void ZenonWindowMinimal3DWidget::dragMoveEvent(QDragMoveEvent * event)
{
	event->ignore();

	const QMimeData * mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragMoveCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		if (false == m_OnDragMoveCallback(glm::vec2(event->pos().x(), event->pos().y()), buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Exception occurs in 'dragMoveEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Unknown exception occurs in 'dragMoveEvent'.");
	}
}

void ZenonWindowMinimal3DWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
	event->ignore();

	if (m_OnDragLeaveCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		if (false == m_OnDragLeaveCallback())
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Exception occurs in 'dragLeaveEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Unknown exception occurs in 'dragLeaveEvent'.");
	}
}

void ZenonWindowMinimal3DWidget::dropEvent(QDropEvent * event)
{
	event->ignore();

	const QMimeData* mimeData = event->mimeData();
	if (mimeData == nullptr)
		return;

	const QByteArray qtByteBuffer = mimeData->data("ZenonEngineMimeData");
	if (qtByteBuffer.isEmpty())
		return;

	if (m_OnDragDropCallback == nullptr)
	{
		event->accept();
		return;
	}

	try
	{
		const CByteBuffer buffer(qtByteBuffer.data(), qtByteBuffer.size());

		if (false == m_OnDragDropCallback(glm::vec2(event->pos().x(), event->pos().y()), buffer))
			return;

		event->accept();
	}
	catch (const CException& e)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Exception occurs in 'dropEvent'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Unknown exception occurs in 'dropEvent'.");
	}
}



//
// Slots
//
void ZenonWindowMinimal3DWidget::onCustomContextMenu(const QPoint& point)
{
	m_ContextMenu->clear();

	if (m_OnContextMenu == nullptr)
		return;

	try
	{
		std::shared_ptr<IPropertiesGroup> propertiesGroup = MakeShared(CPropertiesGroup, "DefaultContextMenuTitle", "DefaultContextMenuDescription");
		if (false == m_OnContextMenu(glm::vec2(point.x(), point.y()), propertiesGroup)) // TODO: try/catch
			return;

		m_ContextMenu = CreateContextMenuFromPropertiesGroup(propertiesGroup);
		m_ContextMenu->popup(mapToGlobal(point));
	}
	catch (const CException& e)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Exception occurs in 'onCustomContextMenu'.");
		Log::Error("--->%s", e.MessageCStr());
	}
	catch (...)
	{
		Log::Error("ZenonWindowMinimal3DWidget: Unknown exception occurs in 'onCustomContextMenu'.");
	}
}