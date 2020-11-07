#include "stdafx.h"

// General
#include "ZenonPropertiesEditorWidget.h"


ZenonPropertiesEditorWidget::ZenonPropertiesEditorWidget(QWidget * parent)
	: QtTreePropertyBrowser(parent)
	, m_CurrentlyConnectedObject(nullptr)
{
	m_VariantManager = MakeShared(QtVariantPropertyManager, this);
	m_GroupPropertyManager = MakeShared(QtGroupPropertyManager, this);

	setHeaderVisible(false);
	setFactoryForManager(m_VariantManager.get(), new QtVariantEditorFactory);

	connect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}

ZenonPropertiesEditorWidget::~ZenonPropertiesEditorWidget()
{
}

void ZenonPropertiesEditorWidget::setTest(std::shared_ptr<ISceneNode> SceneNode)
{
	clear();
	m_VariantManager->clear();
	m_PropertiesMap.clear();

	disconnect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	if (SceneNode)
	{
		auto prop = CreateProperty("SceneNode", SceneNode->GetProperties());
		if (prop)
			addProperty(prop);

		for (const auto& c : SceneNode->GetComponents())
		{
			auto prop = CreateProperty("Component", c.second->GetProperties());
			if (prop)
				addProperty(prop);
		}
	}

	connect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	//connect(obj, SIGNAL(propertyChanged()), this, SLOT(objectUpdated()));
	//objectUpdated();

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
		QtVariantProperty * property = m_VariantManager->addProperty(mp.type(), mp.name());
		if (property == NULL)
			continue;

		property->setEnabled(mp.isWritable());

		m_PropertiesMap[property] = mp.name();
		addProperty(property);
	}
	connect(obj, SIGNAL(propertyChanged()), this, SLOT(objectUpdated()));
	objectUpdated();
}



//
// Slots
//
void ZenonPropertiesEditorWidget::valueChanged(QtProperty * property, const QVariant& value)
{
	Log::Info("Prop '%s'.", property->whatsThis().toStdString().c_str());

	if (m_CurrentlyConnectedObject)
		m_CurrentlyConnectedObject->setProperty(m_PropertiesMap[property].c_str(), value);

	objectUpdated();
}

void ZenonPropertiesEditorWidget::objectUpdated()
{
	//disconnect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	//for (const auto& it : m_PropertiesMap)
	//	m_VariantManager->setValue(it.first, m_CurrentlyConnectedObject->property(it.second.c_str()));

	//connect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}



namespace
{

}



//
// Private
//

QtVariantProperty * ZenonPropertiesEditorWidget::CreateVariantProperty(QVariant::Type PropType, std::string Name, std::string Description)
{
	QtVariantProperty * vartiantProp = m_VariantManager->addProperty(PropType, Name.c_str());
	vartiantProp->setWhatsThis(Name.c_str());
	vartiantProp->setToolTip(Description.c_str());
	return vartiantProp;
}

QtVariantProperty * ZenonPropertiesEditorWidget::CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property)
{
	QtVariantProperty * vartiantProp = m_VariantManager->addProperty(PropType, Property->GetName().c_str());
	vartiantProp->setToolTip(Property->GetDescription().c_str());
	vartiantProp->setWhatsThis(Property->GetName().c_str());
	//vartiantProp->setWhatsThis((ParentPropsPath + "." + vartiantProp->whatsThis().toStdString()).c_str());
	return vartiantProp;
}

QtProperty* ZenonPropertiesEditorWidget::CreateProperty(const std::string& ParentPropsPath, std::shared_ptr<IProperty> Property)
{
	if (auto propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		QtProperty* property = m_GroupPropertyManager->addProperty(propGroup->GetName().c_str());
		property->setWhatsThis((ParentPropsPath + "." + property->propertyName().toStdString()).c_str());

		for (const auto& it : propGroup->GetProperties())
		{
			auto subProperty = CreateProperty(property->whatsThis().toStdString(), it.second);
			if (subProperty)
			{
				subProperty->setWhatsThis(property->whatsThis() + "." + subProperty->propertyName());
				property->addSubProperty(subProperty);
			}
		}

		return property;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::String, propT);
		vartiantProp->setValue(propT->Get().c_str());
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Double, propT);
		vartiantProp->setValue(propT->Get());
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<glm::vec3>>(Property))
	{
		auto propertyGroup = m_GroupPropertyManager->addProperty(propT->GetName().c_str());
		propertyGroup->setWhatsThis((ParentPropsPath + "." + propertyGroup->propertyName().toStdString()).c_str());

		{
			QtVariantProperty * xProp = CreateVariantProperty(QVariant::Type::Double, "X");
			xProp->setValue(propT->Get().x);

			xProp->setWhatsThis(propertyGroup->whatsThis() + "." + xProp->propertyName());
			propertyGroup->addSubProperty(xProp);
		}

		{
			QtVariantProperty * yProp = CreateVariantProperty(QVariant::Type::Double, "Y");
			yProp->setValue(propT->Get().y);

			yProp->setWhatsThis(propertyGroup->whatsThis() + "." + yProp->propertyName());
			propertyGroup->addSubProperty(yProp);
		}

		{
			QtVariantProperty * zProp = CreateVariantProperty(QVariant::Type::Double, "Z");
			zProp->setValue(propT->Get().z);

			zProp->setWhatsThis(propertyGroup->whatsThis() + "." + zProp->propertyName());
			propertyGroup->addSubProperty(zProp);
		}

		return propertyGroup;
	}

	Log::Warn("Unsupported property [%s] type.", Property->GetName().c_str());
	return nullptr;
}