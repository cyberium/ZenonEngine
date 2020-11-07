#include "stdafx.h"

// General
#include "ZenonPropertiesEditorWidget.h"


namespace
{
	QtProperty * CreateProperty(QtGroupPropertyManager * GroupManager, QtVariantPropertyManager * VariantManager, std::shared_ptr<IProperty> Property)
	{
		QtProperty * property = nullptr;
		if (auto propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
		{
			property = GroupManager->addProperty(QString(propGroup->GetName().c_str()));

			for (const auto& it : propGroup->GetProperties())
			{
				auto prop = CreateProperty(GroupManager, VariantManager, it.second);
				if (prop)
					property->addSubProperty(prop);
			}
		}
		else if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
		{
			QtVariantProperty * vartiantProp = VariantManager->addProperty(QVariant::Type::String, propT->GetName().c_str());
			vartiantProp->setValue(propT->Get().c_str());
			property = vartiantProp;
		}
		else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
		{
			QtVariantProperty * vartiantProp = VariantManager->addProperty(QVariant::Type::Double, propT->GetName().c_str());
			vartiantProp->setValue(propT->Get());
			property = vartiantProp;
		}
		else if (auto propT = std::dynamic_pointer_cast<IPropertyT<glm::vec3>>(Property))
		{
			auto propertyGroup = GroupManager->addProperty(QString(propT->GetName().c_str()));

			QtVariantProperty * xProp = VariantManager->addProperty(QVariant::Type::Double, "X");
			xProp->setValue(propT->Get().x);
			propertyGroup->addSubProperty(xProp);

			QtVariantProperty * yProp = VariantManager->addProperty(QVariant::Type::Double, "Y");
			yProp->setValue(propT->Get().y);
			propertyGroup->addSubProperty(yProp);

			QtVariantProperty * zProp = VariantManager->addProperty(QVariant::Type::Double, "Z");
			zProp->setValue(propT->Get().z);
			propertyGroup->addSubProperty(zProp);

			property = propertyGroup;
		}
		else
		{
			Log::Error("Unsupported property [%s] type.", Property->GetName().c_str());
		}

		return property;
	}
}


ZenonPropertiesEditorWidget::ZenonPropertiesEditorWidget(QWidget * parent)
	: QtTreePropertyBrowser(parent)
	, m_CurrentlyConnectedObject(nullptr)
{
	m_VariantManager = new QtVariantPropertyManager(this);
	m_GroupPropertyManager = new QtGroupPropertyManager(this);

	setHeaderVisible(false);
	setFactoryForManager(m_VariantManager, new QtVariantEditorFactory);

}

ZenonPropertiesEditorWidget::~ZenonPropertiesEditorWidget()
{
}

void ZenonPropertiesEditorWidget::setTest(std::shared_ptr<ISceneNode> SceneNode)
{
	clear();
	m_VariantManager->clear();
	m_PropertiesMap.clear();


	if (SceneNode)
	{
		auto prop = CreateProperty(m_GroupPropertyManager, m_VariantManager, SceneNode->GetProperties());
		if (prop)
			addProperty(prop);

		for (const auto& c : SceneNode->GetComponents())
		{
			auto prop = CreateProperty(m_GroupPropertyManager, m_VariantManager, c.second->GetProperties());
			if (prop)
				addProperty(prop);
		}
	}

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

void ZenonPropertiesEditorWidget::valueChanged(QtProperty * property, const QVariant& value)
{
	if (m_CurrentlyConnectedObject)
		m_CurrentlyConnectedObject->setProperty(m_PropertiesMap[property].c_str(), value);

	objectUpdated();
}

void ZenonPropertiesEditorWidget::objectUpdated()
{
	disconnect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));

	for (const auto& it : m_PropertiesMap)
		m_VariantManager->setValue(it.first, m_CurrentlyConnectedObject->property(it.second.c_str()));

	connect(m_VariantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(valueChanged(QtProperty*, QVariant)));
}