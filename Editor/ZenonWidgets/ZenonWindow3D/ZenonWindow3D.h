#pragma once

#include <qframe.h>

#include "EditorInterfaces.h"
#include "../ZenonWindowMinimal3D/ZenonWindowMinimal3D.h"

class ZenonWindow3D
	: public ZenonWindowMinimal3DWidget
{
	Q_OBJECT
public:
	ZenonWindow3D(QWidget * parent);
	virtual ~ZenonWindow3D();

	void SetEditors(IEditor3DFrame* Editor3DFrame, IEditorUIFrame* EditorUIFrame) { m_Editor3D = Editor3DFrame; m_EditorUI = EditorUIFrame; }

private slots:
	void slotCustomMenuRequested(const QPoint& pos);

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
	IEditor3DFrame* m_Editor3D;
	IEditorUIFrame* m_EditorUI;
};
