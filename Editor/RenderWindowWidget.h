#pragma once

class Direct3DWidget
	: public QFrame
	, public IWindowObject
{
	Q_OBJECT

public:
	Direct3DWidget(QWidget *parent = 0);
	~Direct3DWidget();

	QPaintEngine* paintEngine() const override { return nullptr; }

	// IWindowObject
	std::string GetWindowName();
	long GetWindowWidth();   
	long GetWindowHeight();   
	HWND GetHWnd();
	HDC BeginPaint(LPPAINTSTRUCT PaintStruct);
	BOOL EndPaint(LPPAINTSTRUCT PaintStruct);
	BOOL GetClientRect(LPRECT Rect);
	BOOL ClientToScreen(LPPOINT Point);
	BOOL ScreenToClient(LPPOINT Point);
	BOOL BringWindowToTop();
	BOOL ShowWindow(int nCmdShow);
	BOOL UpdateWindow();
	BOOL DestroyWindow();

public slots:
	bool Rendering();

private:
	void paintEvent(QPaintEvent *pEvent) override;
};
