#include "stdafx.h"

// General
#include "MainEditor.h"

#include <qfilesystemmodel.h>

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
	, m_Editor3D(nullptr)
{
	m_UI.setupUi(this);

	m_PropertiesController = std::make_shared<CPropertiesController>(m_UI.PropertyEditor);

	// Unite file browser and log docker
	QMainWindow::tabifyDockWidget(m_UI.DockerFileBrowser, m_UI.DockerLog);

	// Context menu
	connect(m_UI.MainEditor3D, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	m_UI.FSTreeViewer->setModel(fsModel);

	m_PropertiesController->OnSceneNodeSelected(nullptr);
}

MainEditor::~MainEditor()
{
}



//
// IEditorUIFrame
//
void MainEditor::OnSceneNodeSelectedIn3DEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	

	OnSceneNodeSelected(SceneNode3D);
}



//
// IEditorSharedFrame
//
void MainEditor::OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D)
{
	m_PropertiesController->OnSceneNodeSelected(SceneNode3D.get());
}



//
// Slots
//
void MainEditor::slotCustomMenuRequested(const QPoint& pos)
{

}
