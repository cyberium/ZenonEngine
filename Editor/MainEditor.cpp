#include "stdafx.h"

// General
#include "MainEditor.h"

#include <qfilesystemmodel.h>

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
{
	m_UI.setupUi(this);

	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController = std::make_shared<CPropertiesController>(m_UI.PropertyEditor);

	// Unite file browser and log docker
	QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLog);
}

MainEditor::~MainEditor()
{
}

void MainEditor::OnSceneNodeSelectedIn3DEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	OnSceneNodeSelected(SceneNode3D);
}

void MainEditor::OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	getSceneTree()->RefreshTreeViewModel(SceneNode3D);
	m_PropertiesController->OnSceneNodeSelected(SceneNode3D.get());
}


//
// Slots
//

void MainEditor::slotCustomMenuRequested(const QPoint& pos)
{
	/* Create an object context menu */
	QMenu * menu = new QMenu(this);

	/* Create actions to the context menu */
	QAction * editDevice = new QAction(trUtf8("Редактировать"), this);
	QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);

	/* Connect slot handlers for Action pop-up menu */
	connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Call Handler dialog editing
	connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Handler delete records

	/* Set the actions to the menu */
	menu->addAction(editDevice);
	menu->addAction(deleteDevice);

	/* Call the context menu */
	menu->popup(pos);
}

void MainEditor::slotEditRecord()
{
}

void MainEditor::slotRemoveRecord()
{
}
