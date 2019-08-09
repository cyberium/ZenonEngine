#include "stdafx.h"

// General
#include "MainEditor.h"

// Addtionals
#include "SceneNodeTreeModel.h"
#include "RenderWindowWidget.h"

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
{
	ui.setupUi(this);

	Direct3DWidget* xScene = new Direct3DWidget(this);
	xScene->setGeometry(ui.frame->geometry());

	ui.frame = xScene;
}

MainEditor::~MainEditor()
{
}

void MainEditor::ApplyScene(std::shared_ptr<Scene3D> Scene)
{
	m_Scene = Scene;

	//CSceneNodeTreeModel * model = new CSceneNodeTreeModel(Scene);

	//ui.treeView->setHeaderHidden(true);
	//ui.treeView->setRootIsDecorated(true);
	//ui.treeView->setRootIndex(model->index(0, 0));
}
