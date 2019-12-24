#include "stdafx.h"

// General
#include "PropertiesController.h"

// Additional
#include "PropertyEditor/Core/PropertySet.h"
#include "PropertyEditor/Core/PropertyCore.h"
#include "PropertyEditor/Core/PropertyGUI.h"


CPropertiesController::CPropertiesController(QtnPropertyWidget * PropertyWidget)
	: m_PropertyWidget(PropertyWidget)
{


}

CPropertiesController::~CPropertiesController()
{
}

void CPropertiesController::SceneNodeSelected(std::shared_ptr<ISceneNode> SceneNode)
{
	auto m_propertySetRoot = new QtnPropertySet(this);

	/*
	auto m_propertySet0 = new QtnPropertySet(this);
	m_propertySet0->setName("SomePropsGroup");
	m_propertySet0->setDescription("Property group description");

	m_propertySetRoot->addChildProperty(m_propertySet0);

	auto boolValue = qtnCreateProperty<QtnPropertyBool>(m_propertySet0);
	boolValue->setName("BoolValue");
	boolValue->setDescription("Bool value");
	boolValue->setValue(false);

	QVector<QtnEnumValueInfo> enumValues;
	enumValues.push_back(QtnEnumValueInfo(0, "Enum0", "Enum0 display name"));
	enumValues.push_back(QtnEnumValueInfo(1, "Enum1", "Enum1 display name", QtnEnumValueStateObsolete));
	enumValues.push_back(QtnEnumValueInfo(2, "Enum2", "Enum2 display name", QtnEnumValueStateInvalid));
	enumValues.push_back(QtnEnumValueInfo(3, "Enum3", "Enum3 display name"));

	QtnEnumInfo* eInfo = new QtnEnumInfo("TestEnum", enumValues);

	auto enumValue = qtnCreateProperty<QtnPropertyEnum>(m_propertySet0);
	enumValue->setName("EnumValue");
	enumValue->setDescription("Enum value");
	enumValue->setEnumInfo(eInfo);
	enumValue->setValue(2);

	auto pointValue = qtnCreateProperty<QtnPropertyQPoint>(m_propertySet0);
	pointValue->setName("QPointValue");
	pointValue->setDescription("Float value");
	pointValue->setValue(QPoint(123, 123));

	auto sizeValue = qtnCreateProperty<QtnPropertyQSize>(m_propertySet0);
	sizeValue->setName("QSizeValue");
	sizeValue->setDescription("Float value");
	sizeValue->setValue(QSize(123, 123));

	auto floatValue = qtnCreateProperty<QtnPropertyFloat>(m_propertySet0);
	floatValue->setName("FloatValue");
	floatValue->setDescription("Float value");
	floatValue->setMaxValue(1.f);
	floatValue->setMinValue(0.f);
	floatValue->setStepValue(0.1f);
	floatValue->setValue(0.3f);

	auto textColor = qtnCreateProperty<QtnPropertyQColor>(m_propertySetRoot);
	textColor->setName("TextColor");
	textColor->setDescription("Foreground text color");
	textColor->setValue(QColor(0, 0, 0));
	*/

	if (SceneNode)
	{
		CreateProperty(m_propertySetRoot, SceneNode->GetProperties());
	}

	m_PropertyWidget->setPropertySet(m_propertySetRoot);
}

void CPropertiesController::CreateProperty(QtnPropertySet * PropertiesSet, std::shared_ptr<IProperty> Property)
{
	if (std::shared_ptr<IPropertiesGroup> propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		auto propertySet = new QtnPropertySet(this);
		propertySet->setName(propGroup->GetName().c_str());
		propertySet->setDescription(propGroup->GetDescription().c_str());
		PropertiesSet->addChildProperty(propertySet);

		for (const auto& prop : propGroup->GetProperties())
		{
			CreateProperty(propertySet, prop.second);
		}
	}
	else if (std::shared_ptr<IPropertyT<std::string>> propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		auto stringProperty = qtnCreateProperty<QtnPropertyString>(PropertiesSet);
		stringProperty->setName(propT->GetName().c_str());
		stringProperty->setDescription(propT->GetDescription().c_str());
		stringProperty->setValue(propT->Get());

		auto lambda = [stringProperty, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
		{
			if (reason & QtnPropertyChangeReasonValue)
			{
				propT->Set(stringProperty->value());
			}
		};
		QObject::connect(stringProperty, &QtnProperty::propertyDidChange, lambda);
	}
	else if (std::shared_ptr<IPropertyT<glm::vec3>> propT = std::dynamic_pointer_cast<IPropertyT<glm::vec3>>(Property))
	{
		auto vec3Property = qtnCreateProperty<QtnPropertyVec3>(PropertiesSet);
		vec3Property->setName(propT->GetName().c_str());
		vec3Property->setDescription(propT->GetDescription().c_str());
		vec3Property->setValue(propT->Get());

		auto lambda = [vec3Property, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason)
		{
			if (reason & QtnPropertyChangeReasonValue)
			{
				propT->Set(vec3Property->value());
			}
		};
		QObject::connect(vec3Property, &QtnProperty::propertyDidChange, lambda);
	}
	else
	{
		Log::Error("Unsupported property [%s] type.", Property->GetName().c_str());
	}
}
