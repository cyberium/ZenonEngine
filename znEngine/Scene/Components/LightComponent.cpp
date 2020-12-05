#include "stdafx.h"

// General
#include "LightComponent.h"

CLightComponent::CLightComponent(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
	, m_IsEnabled(true)
	, m_IsCastShadows(false)
{
	GetProperties()->SetName("LightComponent");

	std::shared_ptr<CPropertyWrapped<ColorRBG>> ambientColor = MakeShared(CPropertyWrapped<ColorRBG>, "AmbientColor", "descr", glm::vec3(0.2f));
	ambientColor->SetValueSetter(std::bind(&CLightComponent::SetAmbientColor, this, std::placeholders::_1));
	ambientColor->SetValueGetter(std::bind(&CLightComponent::GetAmbientColor, this));
	GetProperties()->AddProperty(ambientColor);

	std::shared_ptr<CPropertyWrappedColor> diffuseColor = MakeShared(CPropertyWrappedColor, "DiffuseColor", "descr", glm::vec3(1.0f));
	diffuseColor->SetValueSetter(std::bind(&CLightComponent::SetColor, this, std::placeholders::_1));
	diffuseColor->SetValueGetter(std::bind(&CLightComponent::GetColor, this));
	GetProperties()->AddProperty(diffuseColor);

	std::shared_ptr<CPropertyWrapped<float>> range = MakeShared(CPropertyWrapped<float>, "Range", "descr", 1000.0f);
	range->SetValueSetter(std::bind(&CLightComponent::SetRange, this, std::placeholders::_1));
	range->SetValueGetter(std::bind(&CLightComponent::GetRange, this));
	GetProperties()->AddProperty(range);

	m_LightStruct = (SGPULight*)_aligned_malloc(sizeof(SGPULight), 16);
	*m_LightStruct = SGPULight();
}

CLightComponent::~CLightComponent()
{
	if (m_LightStruct != nullptr)
		_aligned_free(m_LightStruct);
}



//
// ILightComponent3D
//
void CLightComponent::SetAmbientColor(glm::vec3 Value)
{
	m_LightStruct->AmbientColor = glm::vec4(Value, 1.0f);
}

glm::vec3 CLightComponent::GetAmbientColor() const
{
	return m_LightStruct->AmbientColor.rgb;
}

void CLightComponent::SetColor(glm::vec3 Value)
{
	m_LightStruct->Color = glm::vec4(Value, 1.0f);
}

glm::vec3 CLightComponent::GetColor() const
{
	return m_LightStruct->Color.rgb();
}

void CLightComponent::SetType(ELightType Value)
{
	m_LightStruct->Type = Value;
}

ELightType CLightComponent::GetType() const
{
	return m_LightStruct->Type;
}

void CLightComponent::SetRange(float Value)
{
	m_LightStruct->Range = Value;
}

float CLightComponent::GetRange() const
{
	return m_LightStruct->Range;
}

void CLightComponent::SetIntensity(float Value)
{
	m_LightStruct->Intensity = Value;
}

float CLightComponent::GetIntensity() const
{
	return m_LightStruct->Intensity;
}

void CLightComponent::SetSpotlightAngle(float Value)
{
	m_LightStruct->SpotlightAngle = Value;
}

float CLightComponent::GetSpotlightAngle() const
{
	return m_LightStruct->SpotlightAngle;
}



//
// ILight3D
//
void CLightComponent::SetEnabled(bool Value)
{
	m_IsEnabled = Value;
}

bool CLightComponent::IsEnabled() const
{
	return m_IsEnabled;
}

void CLightComponent::SetCastShadows(bool Value)
{
	m_IsCastShadows = Value;
}

bool CLightComponent::IsCastShadows() const
{
	return m_IsCastShadows;
}

glm::mat4 CLightComponent::GetViewMatrix() const
{
	const glm::vec3& ownerTranslate = GetOwnerNode().GetPosition();
	const glm::vec3& ownerRotation = GetOwnerNode().GetRotationEuler();

	if (m_LightStruct->Type == ELightType::Point)
	{
		throw CException("Not implemented.");
	}
	else if (m_LightStruct->Type == ELightType::Spot)
	{
		return glm::lookAt(ownerTranslate, ownerTranslate + ownerRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (m_LightStruct->Type == ELightType::Directional)
	{
		return glm::lookAt(- glm::normalize(ownerRotation) * 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	throw CException("Unknown light type '%d'.", m_LightStruct->Type);
}

glm::mat4 CLightComponent::GetProjectionMatrix() const
{
	if (m_LightStruct->Type == ELightType::Point)
	{
		throw CException("Not implemented.");
	}
	else if (m_LightStruct->Type == ELightType::Spot)
	{
		return glm::perspectiveRH_ZO(glm::radians(m_LightStruct->SpotlightAngle), 1.0f, 1.0f, m_LightStruct->Range);
	}
	else if (m_LightStruct->Type == ELightType::Directional)
	{
		const float t = 100.0f;
		return glm::orthoRH_ZO<float>(-t, t, -t, t, -200.0f, 200.0f);
	}
	
	throw CException("Unknown light type '%d'.", m_LightStruct->Type);
}

Frustum CLightComponent::GetFrustum() const
{
	Frustum frustum;
	frustum.buildViewFrustum(GetViewMatrix(), GetProjectionMatrix());
	return frustum;
}

const SGPULight& CLightComponent::GetGPULightStruct() const
{
	return *m_LightStruct;
}



//
// ISceneNodeComponent
//
void CLightComponent::Accept(IVisitor* visitor)
{
	if (false == IsEnabled())
		return;

	visitor->Visit(std::dynamic_pointer_cast<ILight3D>(shared_from_this()));
}



//
// IObjectSaveLoad
//

namespace
{
	std::string LightTypeToString(ELightType Type)
	{
		switch (Type)
		{
			case ELightType::Point:
				return "Point";
			case ELightType::Spot:
				return "Spot";
			case ELightType::Directional:
				return "Directional";
		}
		throw CException("Unknown LightType '%d'", (int)Type);
	}

	ELightType LightTypeFromString(std::string Type)
	{
		if (Type == "Point")
			return ELightType::Point;
		else if (Type == "Spot")
			return ELightType::Spot;
		else if (Type == "Directional")
			return ELightType::Directional;
		throw CException("Unknown LightType '%s'", Type.c_str());
	}
}

void CLightComponent::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CLightComponent>(Destination);

	destCast->SetAmbientColor(GetAmbientColor());
	destCast->SetColor(GetColor());
	destCast->SetType(GetType());
	destCast->SetRange(GetRange());
	destCast->SetIntensity(GetIntensity());
	destCast->SetSpotlightAngle(GetSpotlightAngle());
}

void CLightComponent::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	auto lightStruct = Reader->GetChild("LightStruct");
	if (lightStruct == nullptr)
		throw CException("Unable to find 'LightStruct' xml child.");

	{
		auto ambientColor = lightStruct->GetChild("AmbientColor");
		if (ambientColor == nullptr)
			throw CException("Unable to find 'AmbientColor' xml child.");
		SetAmbientColor(ambientColor->GetVec3());
	}

	{
		auto color = lightStruct->GetChild("Color");
		if (color == nullptr)
			throw CException("Unable to find 'Color' xml child.");
		SetColor(color->GetVec3());
	}

	{
		auto type = lightStruct->GetChild("Type");
		if (type == nullptr)
			throw CException("Unable to find 'Type' xml child.");
		SetType(LightTypeFromString(type->GetValue()));
	}

	{
		auto range = lightStruct->GetChild("Range");
		if (range == nullptr)
			throw CException("Unable to find 'Range' xml child.");
		SetRange(range->GetFloat());
	}

	{
		auto intensity = lightStruct->GetChild("Intensity");
		if (intensity == nullptr)
			throw CException("Unable to find 'Intensity' xml child.");
		SetIntensity(intensity->GetFloat());
	}

	{
		auto spotlightAngle = lightStruct->GetChild("SpotlightAngle");
		if (spotlightAngle == nullptr)
			throw CException("Unable to find 'SpotlightAngle' xml child.");
		SetSpotlightAngle(spotlightAngle->GetFloat());
	}
}

void CLightComponent::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	auto lightStruct = Writer->CreateChild("LightStruct");

	{
		auto ambientColor = lightStruct->CreateChild("AmbientColor");
		ambientColor->SetVec3(GetAmbientColor());
	}

	{
		auto color = lightStruct->CreateChild("Color");
		color->SetVec3(GetColor());
	}

	{
		auto type = lightStruct->CreateChild("Type");
		type->SetValue(LightTypeToString(GetType()));
	}

	{
		auto range = lightStruct->CreateChild("Range");
		range->SetFloat(GetRange());
	}

	{
		auto intensity = lightStruct->CreateChild("Intensity");
		intensity->SetFloat(GetIntensity());
	}

	{
		auto spotlightAngle = lightStruct->CreateChild("SpotlightAngle");
		spotlightAngle->SetFloat(GetSpotlightAngle());
	}
}
