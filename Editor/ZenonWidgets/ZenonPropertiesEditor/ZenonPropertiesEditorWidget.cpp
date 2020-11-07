#include "stdafx.h"

// General
#include "ZenonPropertiesEditorWidget.h"


ZenonPropertiesEditorWidget::ZenonPropertiesEditorWidget(QWidget * parent)
	: QtTreePropertyBrowser(parent)
	, m_CurrentlyConnectedObject(nullptr)
{
	m_VariantManager = new QtVariantPropertyManager(this);
	setFactoryForManager(m_VariantManager, new QtVariantEditorFactory);

}

ZenonPropertiesEditorWidget::~ZenonPropertiesEditorWidget()
{
}

void ZenonPropertiesEditorWidget::setActiveObject(QObject *obj)
{
	clear();
	m_VariantManager->clear();
	m_PropertiesMap.clear();

	if (m_CurrentlyConnectedObject)
		m_CurrentlyConnectedObject->disconnect(this);
	m_CurrentlyConnectedObject = obj;

	if (obj == nullptr) 
		return;

	for (int i = 1; i < obj->metaObject()->propertyCount(); i++) 
	{
		QMetaProperty mp = obj->metaObject()->property(i);
		QtVariantProperty *property = m_VariantManager->addProperty(mp.type(), mp.name());
		if (property == NULL)
			continue;

		property->setEnabled(mp.isWritable());

		m_PropertiesMap[property] = mp.name();
		addProperty(property);
	}
	connect(obj, SIGNAL(propertyChanged()), this, SLOT(objectUpdated()));
	objectUpdated();
}

void ZenonPropertiesEditorWidget::valueChanged(QtProperty * property, const QVariant& value)
{
	m_CurrentlyConnectedObject->setProperty(m_PropertiesMap[property].c_str(), value);

	objectUpdated();
}

void ZenonPropertiesEditorWidget::objectUpdated()
{
	disconnect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	for (const auto& it : m_PropertiesMap)
	{
		m_VariantManager->setValue(it.first, m_CurrentlyConnectedObject->property(it.second.c_str()));
	}

	connect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}