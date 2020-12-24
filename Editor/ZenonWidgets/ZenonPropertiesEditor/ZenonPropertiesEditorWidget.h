#pragma once

#include <QObject>
#include <QPushButton>
#include <QMap>
#include <QMetaProperty>

#include "QT\qteditorfactory.h"
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
	void variantValueChanged(QtProperty * Property, const QVariant& Value);
	void colorValueChanged(QtProperty * Property, const QColor& Value);
	void sizeFValueChanged(QtProperty * Property, const QSizeF& Value);

private:
	QtProperty *        CreateGroupProperty(std::shared_ptr<IPropertiesGroup> PropertiesGroup, std::string ParentPropsPath);
	QtVariantProperty * CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property);
	QtProperty *        CreateColorProperty(std::shared_ptr<IProperty> Property);
	QtProperty *        CreateSizeFProperty(std::shared_ptr<IProperty> Property);

	QtProperty*         CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath = "");
	void                DisconnectEvents(std::shared_ptr<IProperty> Root);

	void                UpdateVariantPropertyValue(QtProperty* Property, QVariant Value);
	void                UpdateColorPropertyValue(QtProperty* Property, const ColorRGBA& Color);
	void                UpdateSizeFPropertyValue(QtProperty* Property, const glm::vec2& Size);

private:
	std::shared_ptr<QtVariantEditorFactory>    m_VariantEditorFactory;
	std::shared_ptr<QtColorEditorFactory>      m_ColorEditorFactory;

	std::shared_ptr<QtGroupPropertyManager>    m_GroupPropertyManager;
	std::shared_ptr<QtVariantPropertyManager>  m_VariantPropertyManager;
	std::shared_ptr<QtColorPropertyManager>    m_ColorPropertyManager;
	std::shared_ptr<QtSizeFPropertyManager>    m_SizeFPropertyManager;

	// QtFlagPropertyManager
	//QtEnumPropertyManager
	std::shared_ptr<IPropertiesGroup>          m_EditedProperties;
};