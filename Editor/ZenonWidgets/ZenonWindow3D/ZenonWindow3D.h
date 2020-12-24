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
	
private:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
};
