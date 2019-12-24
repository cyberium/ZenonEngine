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
#include "PropertyEditor/Core/Core/PropertyQSize.h"

#include "PropertyEditor/Core/GUI/PropertyQColor.h"

MainEditor::MainEditor(QWidget* Parent)
	: QMainWindow(Parent)
{
	ui.setupUi(this);

	QMainWindow::tabifyDockWidget(ui.DockerFileBrowser, ui.DockerLog);
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

	auto m_propertySetRoot = new QtnPropertySet(this);
	auto m_propertySet0 = new QtnPropertySet(this);
	m_propertySet0->setName(tr("SomePropsGroup"));
	m_propertySet0->setDescription(tr("Property group description"));

	m_propertySetRoot->addChildProperty(m_propertySet0);

	auto boolValue = qtnCreateProperty<QtnPropertyBool>(m_propertySet0);
	boolValue->setName(tr("BoolValue"));
	boolValue->setDescription(tr("Bool value"));
	boolValue->setValue(false);

	QVector<QtnEnumValueInfo> enumValues;
	enumValues.push_back(QtnEnumValueInfo(0, "Enum0", "Enum0 display name"));
	enumValues.push_back(QtnEnumValueInfo(1, "Enum1", "Enum1 display name", QtnEnumValueStateObsolete));
	enumValues.push_back(QtnEnumValueInfo(2, "Enum2", "Enum2 display name", QtnEnumValueStateInvalid));
	enumValues.push_back(QtnEnumValueInfo(3, "Enum3", "Enum3 display name"));

	QtnEnumInfo* eInfo = new QtnEnumInfo("TestEnum", enumValues);

	auto enumValue = qtnCreateProperty<QtnPropertyEnum>(m_propertySet0);
	enumValue->setName(tr("EnumValue"));
	enumValue->setDescription(tr("Enum value"));
	enumValue->setEnumInfo(eInfo);
	enumValue->setValue(2);

	auto pointValue = qtnCreateProperty<QtnPropertyQPoint>(m_propertySet0);
	pointValue->setName(tr("QPointValue"));
	pointValue->setDescription(tr("Float value"));
	pointValue->setValue(QPoint(123, 123));

	auto sizeValue = qtnCreateProperty<QtnPropertyQSize>(m_propertySet0);
	sizeValue->setName(tr("QSizeValue"));
	sizeValue->setDescription(tr("Float value"));
	sizeValue->setValue(QSize(123, 123));

	auto floatValue = qtnCreateProperty<QtnPropertyFloat>(m_propertySet0);
	floatValue->setName(tr("FloatValue"));
	floatValue->setDescription(tr("Float value"));
	floatValue->setMaxValue(1.f);
	floatValue->setMinValue(0.f);
	floatValue->setStepValue(0.1f);
	floatValue->setValue(0.3f);

	auto textColor = qtnCreateProperty<QtnPropertyQColor>(m_propertySetRoot);
	textColor->setName(tr("TextColor"));
	textColor->setDescription(tr("Foreground text color"));
	textColor->setValue(QColor(0, 0, 0));

	ui.PropertyEditor->setPropertySet(m_propertySetRoot);
}


void MainEditor::onCustomContextMenu(const QPoint &point)
{
	QModelIndex index = ui.SceneTreeViewer->indexAt(point);
	if (index.isValid() && index.row() % 2 == 0) 
	{
		contextMenu->exec(ui.SceneTreeViewer->viewport()->mapToGlobal(point));
	}
}