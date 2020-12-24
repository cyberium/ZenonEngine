#pragma once

#include <qframe.h>

// Actions
typedef std::function<bool(const glm::vec2&, std::shared_ptr<IPropertiesGroup>)> OnWindow3DContextMenuCallback;

// Accept Drag
typedef std::function<bool(const glm::vec2&, bool, const CByteBuffer&)>          OnWindow3DDragEnterCallback;
typedef std::function<bool(const glm::vec2&, const CByteBuffer&)>                OnWindow3DDragMoveCallback;
typedef std::function<bool(const glm::vec2&, const CByteBuffer&)>                OnWindow3DDragDropCallback;
typedef std::function<bool()>                                                    OnWindow3DDragLeaveCallback;


class ZenonWindowMinimal3DWidget
	: public QWidget
	, public IznNativeWindow
	, public IznNativeWindow_WindowsSpecific
{
	Q_OBJECT
public:
	ZenonWindowMinimal3DWidget(QWidget * parent);
	virtual ~ZenonWindowMinimal3DWidget();

	// IznNativeWindow
	void SetWindowTitle(const std::string& WindowName) override;
	std::string GetWindowTitle() const override;
	size_t GetWindowWidth() const override;
	size_t GetWindowHeight() const override;
	void SetCursorPosition(const glm::ivec2& CursorPosition);
	glm::ivec2 GetCursorPosition() const override;
	void ShowCursor();
	void HideCursor();
	void Close();
	void SetEventsListener(IznNativeWindowEventListener* WindowEventsListener);
	void ResetEventsListener();

	// IznNativeWindow_WindowsSpecific
	HWND GetHWnd() const override;

	// Actions
	void SetOnContexMenu(OnWindow3DContextMenuCallback Callback);

	// Accept Drag
	void SetOnDragEnter(OnWindow3DDragEnterCallback Callback);
	void SetOnDragMove(OnWindow3DDragMoveCallback Callback);
	void SetOnDragDrop(OnWindow3DDragDropCallback Callback);
	void SetOnDragLeave(OnWindow3DDragLeaveCallback Callback);

	virtual std::shared_ptr<IImage> TakeScreenshot(IBaseManager& BaseManager);

protected:
	QPaintEngine* paintEngine() const override { return nullptr; } // don't touch this magic!!!

	void focusInEvent(QFocusEvent *event) override;
	void focusOutEvent(QFocusEvent *event) override;
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void moveEvent(QMoveEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	void dropEvent(QDropEvent *event) override;

private slots:
	void onCustomContextMenu(const QPoint& point);

protected:
	glm::ivec2 m_PreviousMousePosition;
	IznNativeWindowEventListener* m_EventListener;

	// Actions
	std::shared_ptr<QMenu> m_ContextMenu;
	OnWindow3DContextMenuCallback m_OnContextMenu;

	// Accept drag
	OnWindow3DDragEnterCallback m_OnDragEnterCallback;
	OnWindow3DDragMoveCallback m_OnDragMoveCallback;
	OnWindow3DDragDropCallback m_OnDragDropCallback;
	OnWindow3DDragLeaveCallback m_OnDragLeaveCallback;
};
