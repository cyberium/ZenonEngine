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
	m_SceneNode.reset();
	clear();
	m_VariantManager->clear();
	m_PropertiesMap.clear();


	disconnect(m_VariantManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	if (SceneNode)
	{
		m_SceneNode = SceneNode;

		auto prop = CreateProperty(m_SceneNode->GetProperties());
		if (prop)
			addProperty(prop);

		for (const auto& c : m_SceneNode->GetComponents())
		{
			auto prop = CreateProperty(c.second->GetProperties());
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

	//if (m_CurrentlyConnectedObject)
	//	m_CurrentlyConnectedObject->disconnect(this);
	//m_CurrentlyConnectedObject = obj;

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

std::pair<std::string, std::string> FirstToken(const std::string& PropertyPath)
{
	auto dotIt = PropertyPath.find_first_of(".");
	if (dotIt == std::string::npos)
		return std::make_pair(PropertyPath, "");
	return std::make_pair(PropertyPath.substr(0, dotIt), PropertyPath.substr(dotIt + 1));
}

std::shared_ptr<IProperty> FindProperty(std::shared_ptr<IProperty> Property, const std::string& PropertyPath)
{
	Log::Info("FindProperty: Property: '%s'. Path: '%s'.", Property->GetName().c_str(), PropertyPath.c_str());
	if (PropertyPath.empty())
		return Property;

	auto token = FirstToken(PropertyPath);

	if (auto propertiesGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		auto propertyInGroup = propertiesGroup->GetProperty(token.first);
		if (propertyInGroup != nullptr)
			return FindProperty(propertyInGroup, token.second);
	}

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

	if (tokenFirst == "SceneNode")
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

	//if (m_CurrentlyConnectedObject)
	//	m_CurrentlyConnectedObject->setProperty(m_PropertiesMap[property].c_str(), value);

	//objectUpdated();
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
	return vartiantProp;
}

QtProperty* ZenonPropertiesEditorWidget::CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath)
{
	if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::String, propT);
		vartiantProp->setValue(propT->Get().c_str());
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<bool>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Bool, propT);
		vartiantProp->setValue(propT->Get());
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Double, propT);
		vartiantProp->setValue(propT->Get());
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