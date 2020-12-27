#include "stdafx.h"

// General
#include "LightComponent.h"

// Additional
#include "Light.h"

CLightComponent::CLightComponent(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
{
	GetProperties()->SetName("LightComponent");

	/*std::shared_ptr<CPropertyWrapped<ColorRGB>> ambientColor = MakeShared(CPropertyWrapped<ColorRGB>, "AmbientColor", "descr", ColorRGB(0.2f));
	ambientColor->SetValueSetter(std::bind(&ILight::SetAmbientColor, GetLight(), std::placeholders::_1));
	ambientColor->SetValueGetter(std::bind(&ILight::GetAmbientColor, GetLight()));
	GetProperties()->AddProperty(ambientColor);

	std::shared_ptr<CPropertyWrapped<ColorRGB>> diffuseColor = MakeShared(CPropertyWrapped<ColorRGB>, "DiffuseColor", "descr", ColorRGB(1.0f));
	diffuseColor->SetValueSetter(std::bind(&ILight::SetColor, GetLight(), std::placeholders::_1));
	diffuseColor->SetValueGetter(std::bind(&ILight::GetColor, GetLight()));
	GetProperties()->AddProperty(diffuseColor);

	std::shared_ptr<CPropertyWrapped<float>> range = MakeShared(CPropertyWrapped<float>, "Range", "descr", 1000.0f);
	range->SetValueSetter(std::bind(&ILight::SetRange, GetLight(), std::placeholders::_1));
	range->SetValueGetter(std::bind(&ILight::GetRange, GetLight()));
	GetProperties()->AddProperty(range);*/
}

CLightComponent::~CLightComponent()
{

}



//
// ILightComponent3D
//
void CLightComponent::SetLight(std::shared_ptr<ILight> Light)
{
	m_Light = Light;

	std::dynamic_pointer_cast<ILightInternal>(m_Light)->SetPosition(GetOwnerNode().GetLocalPosition());
	std::dynamic_pointer_cast<ILightInternal>(m_Light)->SetRotation(GetOwnerNode().GetLocalRotationDirection());
}

std::shared_ptr<ILight> CLightComponent::GetLight() const
{
	return m_Light;
}



//
// ISceneNodeComponent
//
void CLightComponent::OnMessage(const ISceneNodeComponent * Component, ComponentMessageType Message)
{
	if (Message == UUID_OnWorldTransformChanged)
	{
		std::dynamic_pointer_cast<ILightInternal>(m_Light)->SetPosition(GetOwnerNode().GetLocalPosition());
		std::dynamic_pointer_cast<ILightInternal>(m_Light)->SetRotation(GetOwnerNode().GetLocalRotationDirection());
	}
}

void CLightComponent::Accept(IVisitor* visitor)
{
	if (m_Light == nullptr)
		return;

	if (false == m_Light->IsEnabled())
		return;

	visitor->Visit(m_Light);
}



//
// IObjectSaveLoad
//
void CLightComponent::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);
}

void CLightComponent::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto lightStruct = Reader->GetChild("Light");
	if (lightStruct == nullptr)
		throw CException("Unable to find 'Light' xml child.");

	std::shared_ptr<ILight> light = MakeShared(CLight, GetBaseManager());
	std::dynamic_pointer_cast<IObjectLoadSave>(light)->Load(lightStruct);
}

void CLightComponent::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	if (m_Light == nullptr)
		throw CException("Unable to save nullptr light component.");

	auto lightStruct = Writer->CreateChild("Light");
	std::dynamic_pointer_cast<IObjectLoadSave>(m_Light)->Save(lightStruct);
}
