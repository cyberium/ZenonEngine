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
	ZenonPropertiesEditorWidget(QWidget * parent = nullptr);
	virtual ~ZenonPropertiesEditorWidget();

	void SetProperties(std::shared_ptr<IPropertiesGroup> Properties);

private slots:
	void variantValueChanged(QtProperty *property, const QVariant &value);

private:
	QtVariantProperty * CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property);
	QtProperty*         CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath = "");
	void                DisconnectEvents(std::shared_ptr<IProperty> Root);

	void                UpdateVariantPropertyValue(QtProperty* Property, QVariant Value);

private:
	std::shared_ptr<QtVariantEditorFactory>    m_VariantEditorFactory;

	std::shared_ptr<QtVariantPropertyManager>  m_VariantPropertyManager;
	std::shared_ptr<QtGroupPropertyManager>    m_GroupPropertyManager;
	std::shared_ptr<QtColorPropertyManager>    m_ColorPropertyManager;

	std::shared_ptr<IPropertiesGroup>          m_PropertiesGroup;
};