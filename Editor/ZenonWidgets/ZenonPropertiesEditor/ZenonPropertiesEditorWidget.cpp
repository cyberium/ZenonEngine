#include "stdafx.h"


// General
#include "ZenonPropertiesEditorWidget.h"


ZenonPropertiesEditorWidget::ZenonPropertiesEditorWidget(QWidget * parent)
	: QtTreePropertyBrowser(parent)
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
	if (m_SceneNode == SceneNode)
		return;

	m_SceneNode.reset();
	clear();
	m_VariantManager->clear();
	m_PropertiesMap.clear();


	disconnect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	if (SceneNode == nullptr)
		return;

	if (m_SceneNode != nullptr)
	{
		DisconnectEvents(m_SceneNode->GetProperties());

		for (const auto& c : m_SceneNode->GetComponents())
			DisconnectEvents(c.second->GetProperties());
	}


	m_SceneNode = SceneNode;

	if (auto prop = CreateProperty(m_SceneNode->GetProperties()))
		addProperty(prop);

	for (const auto& c : m_SceneNode->GetComponents())
		if (auto prop = CreateProperty(c.second->GetProperties()))
			addProperty(prop);

	connect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}

std::pair<std::string, std::string> FirstToken(const std::string& PropertyPath)
{
	auto dotIt = PropertyPath.find_first_of(".");
	if (dotIt == std::string::npos)
		return std::make_pair(PropertyPath, "");
	return std::make_pair(PropertyPath.substr(0, dotIt), PropertyPath.substr(dotIt + 1));
}

std::shared_ptr<IProperty> FindProperty(std::shared_ptr<IProperty> Property, const std::string& PropertyPath)
{
	//Log::Info("FindProperty: Property: '%s'. Path: '%s'.", Property->GetName().c_str(), PropertyPath.c_str());
	if (PropertyPath.empty())
		return Property;

	auto token = FirstToken(PropertyPath);

	if (auto propertiesGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
		if (auto propertyInGroup = propertiesGroup->GetProperty(token.first))
			return FindProperty(propertyInGroup, token.second);

	return nullptr;
}



//
// Slots
//
void ZenonPropertiesEditorWidget::valueChanged(QtProperty * Property, const QVariant& Value)
{
	std::string fullPropertyPath = Property->whatsThis().toStdString();
	std::string propertyStringValue = Value.toString().toStdString();

	Log::Info("Prop '%s'.", fullPropertyPath.c_str());

	auto token = FirstToken(fullPropertyPath);
	std::string tokenFirst = token.first;
	std::string tokenSecond = token.second;

	if (tokenFirst == "Properties")
	{
		auto property = FindProperty(m_SceneNode->GetProperties(), tokenSecond);
		if (property == nullptr)
			throw CException("Property path '%s' not found.", fullPropertyPath.c_str());
		property->FromString(propertyStringValue);
	}
	else
	{
		const auto& components = m_SceneNode->GetComponents();
		const auto& compIt = std::find_if(components.begin(), components.end(), [&tokenFirst](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIt) {
			return ComponentMapIt.second->GetObjectClassName() == tokenFirst;
		});
		if (compIt == components.end())
			throw CException("Component with className '%s' not found.", tokenFirst.c_str());

		auto componentProperty = FindProperty(compIt->second->GetProperties(), tokenSecond);
		if (componentProperty == nullptr)
			throw CException("Property path '%s' not found.", fullPropertyPath.c_str());
		componentProperty->FromString(propertyStringValue);
	}
}



//
// Private
//

QtVariantProperty * ZenonPropertiesEditorWidget::CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property)
{
	QtVariantProperty * vartiantProp = m_VariantManager->addProperty(PropType, Property->GetName().c_str());
	vartiantProp->setToolTip(Property->GetDescription().c_str());
	vartiantProp->setWhatsThis(Property->GetName().c_str());
	return vartiantProp;
}

QtProperty* ZenonPropertiesEditorWidget::CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath)
{
	if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::String, propT);
		vartiantProp->setValue(propT->Get().c_str());
		propT->SetValueChangedCallback([this, vartiantProp](const std::string& Value) {
			UpdatePropertyValue(vartiantProp, QVariant(Value.c_str()));
		});
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<bool>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Bool, propT);
		vartiantProp->setValue(propT->Get());
		propT->SetValueChangedCallback([this, vartiantProp](const bool& Value) {
			UpdatePropertyValue(vartiantProp, QVariant(Value));
		});
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Double, propT);
		vartiantProp->setValue(propT->Get());
		propT->SetValueChangedCallback([this, vartiantProp](const float& Value) {
			UpdatePropertyValue(vartiantProp, QVariant(Value));
		});
		return vartiantProp;
	}
	else if (auto propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		QtProperty* property = m_GroupPropertyManager->addProperty(propGroup->GetName().c_str());
		std::string whatIsThisName = "";
		if (ParentPropsPath.length() > 0)
			whatIsThisName += ParentPropsPath + ".";
		whatIsThisName += property->propertyName().toStdString();
		property->setWhatsThis(whatIsThisName.c_str());

		const auto& subProperties = propGroup->GetProperties();
		if (subProperties.empty())
		{
			Log::Warn("Property '%s' is emprty IPropertiesGroup.", whatIsThisName.c_str());
			return nullptr;
		}

		for (const auto& it : subProperties)
		{
			auto subProperty = CreateProperty(it.second, property->whatsThis().toStdString());
			if (subProperty)
			{
				subProperty->setWhatsThis(property->whatsThis() + "." + subProperty->propertyName());
				property->addSubProperty(subProperty);
			}
		}

		return property;
	}

	Log::Warn("Unsupported property [%s] type.", Property->GetName().c_str());
	return nullptr;
}

void ZenonPropertiesEditorWidget::DisconnectEvents(std::shared_ptr<IProperty> Root)
{
	if (auto disconnectPropertyCallback = std::dynamic_pointer_cast<IPropertyValueChangedCallback>(Root))
		disconnectPropertyCallback->ResetChangedCallback();

	if (auto propertyGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Root))
		for (const auto& it : propertyGroup->GetProperties())
			DisconnectEvents(it.second);
}

void ZenonPropertiesEditorWidget::UpdatePropertyValue(QtProperty * Property, QVariant Value)
{
	disconnect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	m_VariantManager->setValue(Property, Value);

	connect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}
