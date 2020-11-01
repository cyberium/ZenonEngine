#pragma once

#include <qframe.h>

#include "../ZenonWindowMinimal3D/ZenonWindowMinimal3D.h"

class ZenonWindow3D
	: public ZenonWindowMinimal3DWidget
{
	Q_OBJECT
public:
	ZenonWindow3D(QWidget * parent);
	virtual ~ZenonWindow3D();

	void SetEditor(IEditor* Editor) { m_Editor = Editor; }
	
private slots:
	void onCustomContextMenu(const QPoint& pos);

private:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

	void dropEvent(QDropEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;

private:
	IEditor* m_Editor;
};
