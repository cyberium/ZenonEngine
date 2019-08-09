#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainEditor.h"

class MainEditor : public QMainWindow
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	void ApplyScene(std::shared_ptr<Scene3D> Scene);

	Ui::MainEditorClass getUI() const { return ui; }

private:
	std::shared_ptr<Scene3D> m_Scene;
	Ui::MainEditorClass ui;
};
