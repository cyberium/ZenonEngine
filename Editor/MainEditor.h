#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainEditor.h"

class MainEditor : public QMainWindow
{
	Q_OBJECT

public:
	MainEditor(QWidget *parent = Q_NULLPTR);

	Ui::MainEditorClass getUI() const { return ui; }

private:
	Ui::MainEditorClass ui;
};
