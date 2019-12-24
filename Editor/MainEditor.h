#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainEditor.h"

#include "PropertyEditor/PropertiesController.h"

class MainEditor 
	: public QMainWindow
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	void OnSceneNodeSelected(std::shared_ptr<ISceneNode> SceneNode);

	void ApplyScene(std::shared_ptr<IScene> Scene);
	void ApplyTest();

	Ui::MainEditorClass getUI() const { return ui; }

private slots:
	void onCustomContextMenu(const QPoint& point);

private:
	std::shared_ptr<IScene> m_Scene;
	Ui::MainEditorClass ui;

	std::shared_ptr<QMenu> contextMenu;

	std::shared_ptr<CPropertiesController> m_PropertiesController;
};
