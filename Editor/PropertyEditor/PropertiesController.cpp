#include "stdafx.h"

// General
#include "PropertiesController.h"

// Additional
#include "PropertyEditor/Core/PropertySet.h"
#include "PropertyEditor/Core/PropertyCore.h"
#include "PropertyEditor/Core/PropertyGUI.h"


CPropertiesController::CPropertiesController(QtnPropertyWidget * PropertyWidget)
	: m_PropertyWidget(PropertyWidget)
	, m_PropertiesSet(nullptr)
{


}

CPropertiesController::~CPropertiesController()
{
}

void DoRemovePropsFromPropertySet(QtnPropertySet* Set)
{
	auto childs = Set->childProperties();
	for (auto p = childs.begin(); p != childs.end(); )
	{
		if (auto childPropsSet = dynamic_cast<QtnPropertySet*>(*p))
			DoRemovePropsFromPropertySet(childPropsSet);

		if (!Set->removeChildProperty(*p))
			throw CException("Unable to remove property");

		delete *p;

		p = childs.erase(p);
	}
}

void CPropertiesController::OnSceneNodeSelected(ISceneNode3D* SceneNode)
{
	if (m_PropertiesSet)
	{
		DoRemovePropsFromPropertySet(m_PropertiesSet);
		delete m_PropertiesSet;
	}

	m_PropertiesSet = new QtnPropertySet(this);

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
		CreateProperty(m_PropertiesSet, SceneNode->GetProperties());

		for (const auto& c : SceneNode->GetComponents())
		{
			CreateProperty(m_PropertiesSet, c.second->GetProperties());
		}
	}

	m_PropertyWidget->setPropertySet(m_PropertiesSet);
}

void CPropertiesController::CreateProperty(QtnPropertySet* PropertiesSet, const std::shared_ptr<IProperty>& Property)
{
	if (auto propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		auto propertySet = new QtnPropertySet(this);
		PropertiesSet->addChildProperty(propertySet);
		propertySet->setName(propGroup->GetName().c_str());
		propertySet->setDescription(propGroup->GetDescription().c_str());
		
		for (const auto& prop : propGroup->GetProperties())
			CreateProperty(propertySet, prop.second);
	}
	else if (auto act = std::dynamic_pointer_cast<IPropertyAction>(Property))
	{
		auto actionProperty = new QtnPropertyButton(PropertiesSet);
		PropertiesSet->addChildProperty(actionProperty);
		actionProperty->setName(act->GetName().c_str());
		actionProperty->setDescription(act->GetDescription().c_str());
		actionProperty->setClickHandler([act](const QtnPropertyButton* Button) {
			act->ExecuteAction();
		});
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		auto stringProperty = new QtnPropertyString(PropertiesSet);
		PropertiesSet->addChildProperty(stringProperty);

		stringProperty->setName(propT->GetName().c_str());
		stringProperty->setDescription(propT->GetDescription().c_str());
		stringProperty->setValue(propT->Get());

		// From engine to editor
		propT->SetValueChangedCallback(
			[stringProperty](const std::string& value) {  
				stringProperty->setValue(value); 
			}
		);

		// From editor to engine
		QObject::connect(
			stringProperty, 
			&QtnProperty::propertyDidChange, 
			[stringProperty, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason) {
				if (reason & QtnPropertyChangeReasonValue)
					propT->Set(stringProperty->value(), true);
			}
		);
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
	{
		auto floatProperty = new QtnPropertyFloat(PropertiesSet);
		PropertiesSet->addChildProperty(floatProperty);

		floatProperty->setName(propT->GetName().c_str());
		floatProperty->setDescription(propT->GetDescription().c_str());
		floatProperty->setValue(propT->Get());
		floatProperty->setStepValue(0.1f);

		// From engine to editor
		propT->SetValueChangedCallback(
			[floatProperty](const float& value) {  
				floatProperty->setValue(value); 
			}
		);

		// From editor to engine
		QObject::connect(
			floatProperty, 
			&QtnProperty::propertyDidChange, 
			[floatProperty, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason) {
				if (reason & QtnPropertyChangeReasonValue)
					propT->Set(floatProperty->value(), true);
			}
		);
	}
	else if (auto propT = std::dynamic_pointer_cast<CPropertyWrappedColor>(Property))
	{
		QtnPropertyQColor* colorProperty = qtnCreateProperty<QtnPropertyQColor>(PropertiesSet);
		colorProperty->setName(propT->GetName().c_str());
		colorProperty->setDescription(propT->GetDescription().c_str());

		QColor color;
		color.setRgbF(propT->Get().r, propT->Get().g, propT->Get().b);
		colorProperty->setValue(color);

		// From engine to editor
		propT->SetValueChangedCallback(
			[colorProperty](const glm::vec3& value) {

			QColor color;
			color.setRgbF(value.r, value.g, value.b);
			colorProperty->setValue(color);
		}
		);

		// From editor to engine
		QObject::connect(
			colorProperty,
			&QtnProperty::propertyDidChange,
			[colorProperty, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason) {
			if (reason & QtnPropertyChangeReasonValue)
			{
				double r, g, b;
				colorProperty->value().getRgbF(&r, &g, &b);
				propT->Set(glm::vec3(r, g, b), true);
			}
		}
		);
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<glm::vec3>>(Property))
	{
		QtnPropertyVec3* vec3Property = qtnCreateProperty<QtnPropertyVec3>(PropertiesSet);
		vec3Property->setName(propT->GetName().c_str());
		vec3Property->setDescription(propT->GetDescription().c_str());
		vec3Property->setValue(propT->Get());

		// From engine to editor
		propT->SetValueChangedCallback(
			[vec3Property](const glm::vec3& value) {
				vec3Property->setValue(value); 
			}
		);

		// From editor to engine
		QObject::connect(
			vec3Property, 
			&QtnProperty::propertyDidChange, 
			[vec3Property, propT](const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason) {
				if (reason & QtnPropertyChangeReasonValue)
					propT->Set(vec3Property->value(), true);
			}
		);
	}
	else
	{
		Log::Error("Unsupported property [%s] type.", Property->GetName().c_str());
	}
}

