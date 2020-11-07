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

public slots:
	void objectUpdated();

private:
	QtVariantPropertyManager *       m_VariantManager;
	QtGroupPropertyManager * m_GroupPropertyManager;

	QObject *                        m_CurrentlyConnectedObject;
	std::map<QtProperty *, std::string> m_PropertiesMap;
};