#include "stdafx.h"


// General
#include "ZenonPropertiesEditorWidget.h"


ZenonPropertiesEditorWidget::ZenonPropertiesEditorWidget(QWidget * parent)
	: QtTreePropertyBrowser(parent)
{
	m_GroupPropertyManager = MakeShared(QtGroupPropertyManager, this);

	m_VariantEditorFactory = MakeShared(QtVariantEditorFactory);
	m_VariantPropertyManager = MakeShared(QtVariantPropertyManager, this);
	setFactoryForManager(m_VariantPropertyManager.get(), m_VariantEditorFactory.get());

	m_ColorEditorFactory = MakeShared(QtColorEditorFactory, this);
	m_ColorPropertyManager = MakeShared(QtColorPropertyManager, this);
	setFactoryForManager(m_ColorPropertyManager.get(), m_ColorEditorFactory.get());

	m_SizeFPropertyManager = MakeShared(QtSizeFPropertyManager, this);

	setHeaderVisible(true);
}

ZenonPropertiesEditorWidget::~ZenonPropertiesEditorWidget()
{
	unsetFactoryForManager(m_VariantPropertyManager.get());
	unsetFactoryForManager(m_ColorPropertyManager.get());
}

void ZenonPropertiesEditorWidget::SetProperties(std::shared_ptr<IPropertiesGroup> Properties)
{
	if (m_EditedProperties == Properties)
		return;

	m_EditedProperties.reset();

	this->clear();
	m_VariantPropertyManager->clear();
	m_GroupPropertyManager->clear();
	m_ColorPropertyManager->clear();

	//disconnect(m_VariantPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(variantValueChanged(QtProperty*, QVariant)));
	//disconnect(m_ColorPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(colorValueChanged(QtProperty*, QColor)));
	//disconnect(m_SizeFPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QSizeF)), this, SLOT(sizeFValueChanged(QtProperty*, QSizeF)));

	if (Properties == nullptr)
		return;

	if (m_EditedProperties != nullptr)
		for (const auto& property : m_EditedProperties->GetProperties())
			DisconnectEvents(property.second);

	m_EditedProperties = Properties;

	for (const auto& property : m_EditedProperties->GetProperties())
		if (auto qtProperty = CreateProperty(property.second))
			addProperty(qtProperty);
	
	//connect(m_VariantPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(variantValueChanged(QtProperty*, QVariant)));
	//connect(m_ColorPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(colorValueChanged(QtProperty*, QColor)));
	//connect(m_SizeFPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QSizeF)), this, SLOT(sizeFValueChanged(QtProperty*, QSizeF)));
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
void ZenonPropertiesEditorWidget::variantValueChanged(QtProperty * Property, const QVariant& Value)
{
	std::string fullPropertyPath = Property->whatsThis().toStdString();
	std::string propertyStringValue = Value.toString().toStdString();

	Log::Info("PropertiesEditorWidget: Variant property '%s' value '%s'.", fullPropertyPath.c_str(), propertyStringValue.c_str());

	auto property = FindProperty(m_EditedProperties, fullPropertyPath);
	if (property == nullptr)
		throw CException("Property '%s' not found.", fullPropertyPath.c_str());
	property->FromString(propertyStringValue);
}

void ZenonPropertiesEditorWidget::colorValueChanged(QtProperty * Property, const QColor& Value)
{
	std::string fullPropertyPath = Property->whatsThis().toStdString();
	
	qreal r, g, b, a;
	Value.getRgbF(&r, &g, &b, &a);

	Log::Info("PropertiesEditorWidget: Color property '%s' value '%d, %d, %d, %d'.", fullPropertyPath.c_str(), r, g, b, a);

	auto property = FindProperty(m_EditedProperties, fullPropertyPath);
	if (property == nullptr)
		throw CException("Property '%s' not found.", fullPropertyPath.c_str());

	if (auto propertyAsColor3 = std::dynamic_pointer_cast<IPropertyT<ColorRGB>>(property))
	{
		propertyAsColor3->Set(ColorRGB(r, g, b));
	}
	else if (auto propertyAsColor4 = std::dynamic_pointer_cast<IPropertyT<ColorRGBA>>(property))
	{
		propertyAsColor4->Set(ColorRGBA(r, g, b, a));
	}
	else
		throw CException("Unable to cast in 'colorValueChanged'.");
}

void ZenonPropertiesEditorWidget::sizeFValueChanged(QtProperty * Property, const QSizeF & Value)
{
	std::string fullPropertyPath = Property->whatsThis().toStdString();

	auto width = Value.width();
	auto height = Value.height();
	
	Log::Info("PropertiesEditorWidget: SizeF property '%s' value '%f, %f'.", fullPropertyPath.c_str(), width, height);

	auto property = FindProperty(m_EditedProperties, fullPropertyPath);
	if (property == nullptr)
		throw CException("Property '%s' not found.", fullPropertyPath.c_str());

	if (auto propertyVec2 = std::dynamic_pointer_cast<IPropertyT<glm::vec2>>(property))
		propertyVec2->Set(glm::vec2(width, height));
	else
		throw CException("Unable to cast in 'sizeFValueChanged'.");
}


//
// Private
//
QtProperty * ZenonPropertiesEditorWidget::CreateGroupProperty(std::shared_ptr<IPropertiesGroup> PropertiesGroup, std::string ParentPropsPath)
{
	QtProperty* property = m_GroupPropertyManager->addProperty(PropertiesGroup->GetName().c_str());
	std::string whatIsThisName = "";
	if (ParentPropsPath.length() > 0)
		whatIsThisName += ParentPropsPath + ".";
	whatIsThisName += property->propertyName().toStdString();
	property->setWhatsThis(whatIsThisName.c_str());

	return property;
}

QtVariantProperty * ZenonPropertiesEditorWidget::CreateVariantProperty(QVariant::Type PropType, std::shared_ptr<IProperty> Property)
{
	QtVariantProperty * variantProp = m_VariantPropertyManager->addProperty(PropType, Property->GetName().c_str());
	variantProp->setToolTip(Property->GetDescription().c_str());
	variantProp->setWhatsThis(Property->GetName().c_str());
	variantProp->setEnabled(false == Property->IsReadOnly());
	return variantProp;
}

QtProperty * ZenonPropertiesEditorWidget::CreateColorProperty(std::shared_ptr<IProperty> Property)
{
	QtProperty * colorProperty = m_ColorPropertyManager->addProperty(Property->GetName().c_str());

	for (const auto subProp : colorProperty->subProperties())
		colorProperty->removeSubProperty(subProp);

	colorProperty->setToolTip(Property->GetDescription().c_str());
	colorProperty->setWhatsThis(Property->GetName().c_str());
	colorProperty->setEnabled(false == Property->IsReadOnly());
	return colorProperty;
}

QtProperty * ZenonPropertiesEditorWidget::CreateSizeFProperty(std::shared_ptr<IProperty> Property)
{
	QtProperty * sizeFProperty = m_SizeFPropertyManager->addProperty(Property->GetName().c_str());

	//for (const auto subProp : colorProperty->subProperties())
	//	colorProperty->removeSubProperty(subProp);

	sizeFProperty->setToolTip(Property->GetDescription().c_str());
	sizeFProperty->setWhatsThis(Property->GetName().c_str());
	sizeFProperty->setEnabled(false == Property->IsReadOnly());
	return sizeFProperty;
}

QtProperty* ZenonPropertiesEditorWidget::CreateProperty(std::shared_ptr<IProperty> Property, std::string ParentPropsPath)
{
	if (auto propT = std::dynamic_pointer_cast<IPropertyT<std::string>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::String, propT);
		
		UpdateVariantPropertyValue(vartiantProp, QVariant(propT->Get().c_str()));
		
		propT->SetValueChangedCallback([this, vartiantProp](const std::string& Value) {
			UpdateVariantPropertyValue(vartiantProp, QVariant(Value.c_str()));
		});
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<bool>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Bool, propT);
		
		UpdateVariantPropertyValue(vartiantProp, QVariant(propT->Get()));
		
		propT->SetValueChangedCallback([this, vartiantProp](const bool& Value) {
			UpdateVariantPropertyValue(vartiantProp, QVariant(Value));
		});
		return vartiantProp;
	}
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<float>>(Property))
	{
		QtVariantProperty * vartiantProp = CreateVariantProperty(QVariant::Type::Double, propT);

		UpdateVariantPropertyValue(vartiantProp, QVariant(propT->Get()));

		propT->SetValueChangedCallback([this, vartiantProp](const float& Value) {
			UpdateVariantPropertyValue(vartiantProp, QVariant(Value));
		});
		return vartiantProp;
	}

	// Size
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<glm::vec2>>(Property))
	{
		QtProperty* sizeFProperty = CreateSizeFProperty(propT);
		
		UpdateSizeFPropertyValue(sizeFProperty, propT->Get());

		propT->SetValueChangedCallback([this, sizeFProperty](const glm::vec2& Value) {
			UpdateSizeFPropertyValue(sizeFProperty, Value);
		});
		return sizeFProperty;
	}

	// Color RGB
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<ColorRGB>>(Property))
	{
		QtProperty* colorProperty = CreateColorProperty(propT);

		UpdateColorPropertyValue(colorProperty, ColorRGBA(propT->Get(), 1.0f));

		propT->SetValueChangedCallback([this, colorProperty](const ColorRGB& Value) {
			UpdateColorPropertyValue(colorProperty, ColorRGBA(Value, 1.0f));
		});
		return colorProperty;
	}


	// Color RGBA
	else if (auto propT = std::dynamic_pointer_cast<IPropertyT<ColorRGBA>>(Property))
	{
		QtProperty* colorProperty = CreateColorProperty(propT);

		UpdateColorPropertyValue(colorProperty, propT->Get());

		propT->SetValueChangedCallback([this, colorProperty](const ColorRGBA& Value) {
			UpdateColorPropertyValue(colorProperty, Value);
		});
		return colorProperty;
	}

	else if (auto propGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		QtProperty * qtPropetiesGroup = CreateGroupProperty(propGroup, ParentPropsPath);

		const auto& subProperties = propGroup->GetProperties();
		if (subProperties.empty())
		{
			Log::Warn("Property '%s' is empty IPropertiesGroup.", propGroup->GetName().c_str());
			return nullptr;
		}

		for (const auto& it : subProperties)
		{
			if (auto subProperty = CreateProperty(it.second, qtPropetiesGroup->whatsThis().toStdString()))
			{
				subProperty->setWhatsThis(qtPropetiesGroup->whatsThis() + "." + subProperty->propertyName());
				qtPropetiesGroup->addSubProperty(subProperty);
			}
		}

		return qtPropetiesGroup;
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

void ZenonPropertiesEditorWidget::UpdateVariantPropertyValue(QtProperty * Property, QVariant Value)
{
	disconnect(m_VariantPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(variantValueChanged(QtProperty*, QVariant)));
	{
		m_VariantPropertyManager->setValue(Property, Value);
	}
	connect(m_VariantPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QVariant)), this, SLOT(variantValueChanged(QtProperty*, QVariant)));
}

void ZenonPropertiesEditorWidget::UpdateColorPropertyValue(QtProperty * Property, const ColorRGBA& Color)
{
	disconnect(m_ColorPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(colorValueChanged(QtProperty*, QColor)));
	{
		QColor qtColor;
		qtColor.setRgbF(Color.r, Color.g, Color.b, Color.a);
		m_ColorPropertyManager->setValue(Property, qtColor);
	}
	connect(m_ColorPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QColor)), this, SLOT(colorValueChanged(QtProperty*, QColor)));
}

void ZenonPropertiesEditorWidget::UpdateSizeFPropertyValue(QtProperty * Property, const glm::vec2& Size)
{
	disconnect(m_SizeFPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QSizeF)), this, SLOT(sizeFValueChanged(QtProperty*, QSizeF)));
	{
		QSizeF qtSizeF;
		qtSizeF.setWidth(Size.x);
		qtSizeF.setHeight(Size.y);
		m_SizeFPropertyManager->setValue(Property, qtSizeF);
	}
	connect(m_SizeFPropertyManager.get(), SIGNAL(valueChanged(QtProperty*, QSizeF)), this, SLOT(sizeFValueChanged(QtProperty*, QSizeF)));
}
