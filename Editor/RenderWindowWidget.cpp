#include "stdafx.h"

// General
#include "RenderWindowWidget.h"


Direct3DWidget::Direct3DWidget(QWidget *parent /* = 0 */) 
	: QFrame(parent)
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
}
Direct3DWidget::~Direct3DWidget()
{
}

std::string Direct3DWidget::GetWindowName()
{
	return std::string();
}

long Direct3DWidget::GetWindowWidth()
{
	return geometry().width();
}

long Direct3DWidget::GetWindowHeight()
{
	return geometry().height();
}

HWND Direct3DWidget::GetHWnd()
{
	return (HWND)winId();
}

HDC Direct3DWidget::BeginPaint(LPPAINTSTRUCT PaintStruct)
{
	return HDC();
}

BOOL Direct3DWidget::EndPaint(LPPAINTSTRUCT PaintStruct)
{
	return 0;
}

BOOL Direct3DWidget::GetClientRect(LPRECT Rect)
{
	Rect->left = geometry().x();
	Rect->right = geometry().x() + geometry().width();

	Rect->top = geometry().y();
	Rect->bottom = geometry().y() + geometry().height();

	return TRUE;
}

BOOL Direct3DWidget::ClientToScreen(LPPOINT Point)
{
	return 0;
}

BOOL Direct3DWidget::ScreenToClient(LPPOINT Point)
{
	return 0;
}

BOOL Direct3DWidget::BringWindowToTop()
{
	return 0;
}

BOOL Direct3DWidget::ShowWindow(int nCmdShow)
{
	return 0;
}

BOOL Direct3DWidget::UpdateWindow()
{
	return 0;
}

BOOL Direct3DWidget::DestroyWindow()
{
	return 0;
}

bool Direct3DWidget::Rendering()
{

	return true;
}

void Direct3DWidget::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent);

	Rendering();
}
