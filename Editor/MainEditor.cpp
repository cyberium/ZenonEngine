#include "stdafx.h"
#include "MainEditor.h"

#include "RenderWindowWidget.h"

MainEditor::MainEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	Direct3DWidget* xScene = new Direct3DWidget(this);
	xScene->setGeometry(ui.frame->geometry());

	ui.frame = xScene;
}
