#include "stdafx.h"

// General
#include "MainEditor.h"

// Addtionals
#include "SceneNodeTreeModel.h"
#include "RenderWindowWidget.h"

#include <qfilesystemmodel.h>

#include "PropertyEditor/Core/PropertySet.h"


#include "PropertyEditor/Core/Core/PropertyBool.h"
#include "PropertyEditor/Core/Core/PropertyDouble.h"
#include "PropertyEditor/Core/Core/PropertyEnum.h"
#include "PropertyEditor/Core/Core/PropertyEnumFlags.h"
#include "PropertyEditor/Core/Core/PropertyFloat.h"
#include "PropertyEditor/Core/Core/PropertyInt.h"
#include "PropertyEditor/Core/Core/PropertyQPoint.h"


#include "PropertyEditor/Core/GUI/PropertyQColor.h"

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
	ui.SceneTreeViewer->setModel(model);

	contextMenu = std::make_shared<QMenu>(this);
	QAction* uninstallAction = new QAction("Uninstall TA", contextMenu.get());
	contextMenu->addAction(uninstallAction);

	contextMenu->addSeparator();
	
	QAction* uninstallAction33 = new QAction("Uninstall TA33", contextMenu.get());
	contextMenu->addAction(uninstallAction33);

	ui.SceneTreeViewer->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.SceneTreeViewer, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));
}

void MainEditor::ApplyTest()
{
	QFileSystemModel* fsModel = new QFileSystemModel(this);
	fsModel->setRootPath("D:\\_programming\\ZenonEngine\\gamedata");
	ui.FSTreeViewer->setModel(fsModel);

	auto m_propertySet = new QtnPropertySet(this);

	auto boolValue = qtnCreateProperty<QtnPropertyBool>(m_propertySet);
	boolValue->setName(tr("Value"));
	boolValue->setDescription(tr("Float value"));
	boolValue->setValue(false);

	auto floatValue = qtnCreateProperty<QtnPropertyFloat>(m_propertySet);
	floatValue->setName(tr("Value"));
	floatValue->setDescription(tr("Float value"));
	floatValue->setMaxValue(1.f);
	floatValue->setMinValue(0.f);
	floatValue->setStepValue(0.1f);
	floatValue->setValue(0.3f);

	auto textColor = qtnCreateProperty<QtnPropertyQColor>(m_propertySet);
	textColor->setName(tr("TextColor"));
	textColor->setDescription(tr("Foreground text color"));
	textColor->setValue(QColor(0, 0, 0));

	ui.PropertyEditor->setPropertySet(m_propertySet);

}


void MainEditor::onCustomContextMenu(const QPoint &point)
{
	QModelIndex index = ui.SceneTreeViewer->indexAt(point);
	if (index.isValid() && index.row() % 2 == 0) 
	{
		contextMenu->exec(ui.SceneTreeViewer->viewport()->mapToGlobal(point));
	}
}