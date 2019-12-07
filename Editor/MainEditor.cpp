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

}

MainEditor::~MainEditor()
{
}

void MainEditor::ApplyScene(std::shared_ptr<IScene> Scene)
{
	m_Scene = Scene;

	CSceneNodeTreeModel * model = new CSceneNodeTreeModel(Scene);
	ui.treeView->setModel(model);

	contextMenu = std::make_shared<QMenu>(this);
	QAction* uninstallAction = new QAction("Uninstall TA", contextMenu.get());
	contextMenu->addAction(uninstallAction);

	contextMenu->addSeparator();
	
	QAction* uninstallAction33 = new QAction("Uninstall TA33", contextMenu.get());
	contextMenu->addAction(uninstallAction33);

	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}


void MainEditor::onCustomContextMenu(const QPoint &point)
{
	QModelIndex index = ui.treeView->indexAt(point);
	if (index.isValid() && index.row() % 2 == 0) 
	{
		contextMenu->exec(ui.treeView->viewport()->mapToGlobal(point));
	}
}