#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainEditor.h"

class MainEditor : public QMainWindow
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	void ApplyScene(std::shared_ptr<IScene> Scene);

	Ui::MainEditorClass getUI() const { return ui; }

private slots:
	void onCustomContextMenu(const QPoint &point);

private:
	std::shared_ptr<IScene> m_Scene;
	Ui::MainEditorClass ui;

	std::shared_ptr<QMenu> contextMenu;
};
