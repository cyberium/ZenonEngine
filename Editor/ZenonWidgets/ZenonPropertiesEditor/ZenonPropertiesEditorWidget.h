#pragma once

#include <QObject>
#include <QPushButton>
#include <QMap>
#include <QMetaProperty>

#include "QT\qtpropertymanager.h"
#include "QT\qtvariantproperty.h"
#include "QT\qttreepropertybrowser.h"

class ZenonPropertiesEditorWidget
	: public QtTreePropertyBrowser
{
	Q_OBJECT
public:
	ZenonPropertiesEditorWidget(QWidget *parent = 0);
	virtual ~ZenonPropertiesEditorWidget();

	void setTest(std::shared_ptr<ISceneNode> SceneNode);
	void setActiveObject(QObject *obj);

private slots:
	void valueChanged(QtProperty *property, const QVariant &value);
	void objectUpdated();

private:
	QtVariantProperty * CreateVariantProperty(QVariant::Type PropType, std::string Name, std::string Description = "");
	QtVariantProperty * CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property);
	QtProperty*         CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath = "");

private:
	std::shared_ptr<QtVariantPropertyManager>  m_VariantManager;
	std::shared_ptr<QtGroupPropertyManager>    m_GroupPropertyManager;
	std::shared_ptr<ISceneNode>                m_SceneNode;
	std::map<QtProperty *, std::string>        m_PropertiesMap;
};