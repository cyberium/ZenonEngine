#include "stdafx.h"

// General
#include "LightComponent3D.h"

CLightComponent3D::CLightComponent3D(const ISceneNode& OwnerNode)
    : CComponentBase(OwnerNode)
{
	GetProperties()->SetName("LightComponent");

	std::shared_ptr<CPropertyWrapped<ColorRBG>> ambientColor = MakeShared(CPropertyWrapped<ColorRBG>, "AmbientColor", "Ambient color part of the light.");
	ambientColor->SetValueSetter(std::bind(&CLightComponent3D::SetAmbientColor, this, std::placeholders::_1));
	ambientColor->SetValueGetter(std::bind(&CLightComponent3D::GetAmbientColor, this));
	GetProperties()->AddProperty(ambientColor);

	std::shared_ptr<CPropertyWrappedColor> diffuseColor = MakeShared(CPropertyWrappedColor, "DiffuseColor", "Ambient color part of the light.");
	diffuseColor->SetValueSetter(std::bind(&CLightComponent3D::SetColor, this, std::placeholders::_1));
	diffuseColor->SetValueGetter(std::bind(&CLightComponent3D::GetColor, this));
	GetProperties()->AddProperty(diffuseColor);

	std::shared_ptr<CPropertyWrapped<float>> range = MakeShared(CPropertyWrapped<float>, "Range", "Range of this light.");
	range->SetValueSetter(std::bind(&CLightComponent3D::SetRange, this, std::placeholders::_1));
	range->SetValueGetter(std::bind(&CLightComponent3D::GetRange, this));
	GetProperties()->AddProperty(range);

	m_LightStruct = (SLight*)_aligned_malloc(sizeof(SLight), 16);
	*m_LightStruct = SLight();
}

CLightComponent3D::~CLightComponent3D()
{
	if (m_LightStruct != nullptr)
		_aligned_free(m_LightStruct);
}



//
// ILightComponent3D
//
void CLightComponent3D::SetAmbientColor(glm::vec3 Value)
{
	m_LightStruct->AmbientColor = glm::vec4(Value, 1.0f);
}

glm::vec3 CLightComponent3D::GetAmbientColor() const
{
	return m_LightStruct->AmbientColor.rgb;
}

void CLightComponent3D::SetColor(glm::vec3 Value)
{
	m_LightStruct->Color = glm::vec4(Value, 1.0f);
}

glm::vec3 CLightComponent3D::GetColor() const
{
	return m_LightStruct->Color.rgb();
}

void CLightComponent3D::SetType(ELightType Value)
{
	m_LightStruct->Type = Value;
}

ELightType CLightComponent3D::GetType() const
{
	return m_LightStruct->Type;
}

void CLightComponent3D::SetRange(float Value)
{
	m_LightStruct->Range = Value;
}

float CLightComponent3D::GetRange() const
{
	return m_LightStruct->Range;
}

void CLightComponent3D::SetIntensity(float Value)
{
	m_LightStruct->Intensity = Value;
}

float CLightComponent3D::GetIntensity() const
{
	return m_LightStruct->Intensity;
}

void CLightComponent3D::SetSpotlightAngle(float Value)
{
	m_LightStruct->SpotlightAngle = Value;
}

float CLightComponent3D::GetSpotlightAngle() const
{
	return m_LightStruct->SpotlightAngle;
}



//
// ILight3D
//
glm::mat4 CLightComponent3D::GetViewMatrix() const
{
	const glm::vec3& ownerTranslate = GetOwnerNode().GetTranslation();
	const glm::vec3& ownerRotation = GetOwnerNode().GetRotation();

	if (m_LightStruct->Type == ELightType::Point)
	{
		return glm::mat4(1.0f);
	}
	else if (m_LightStruct->Type == ELightType::Spot)
	{
		return glm::lookAt(ownerTranslate, ownerTranslate + ownerRotation * 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (m_LightStruct->Type == ELightType::Directional)
	{
		//glm::vec3 position = -m_LightStruct->DirectionWS.xyz();
		//position *= 100.0f;
		return glm::lookAt(glm::vec3(10.0f), glm::vec3(10.0f) + ownerRotation * 100.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return glm::mat4(1.0f);
}

glm::mat4 CLightComponent3D::GetProjectionMatrix() const
{
	if (m_LightStruct->Type == ELightType::Point)
	{
		return glm::mat4(1.0f);
	}
	else if (m_LightStruct->Type == ELightType::Spot)
	{
		return glm::perspective(glm::radians(m_LightStruct->SpotlightAngle), 1.0f, 1.0f, m_LightStruct->Range);
	}
	else if (m_LightStruct->Type == ELightType::Directional)
	{
		const float t = 500.0f;
		return glm::ortho<float>(-t, t, -t, t, -500, 500);
	}
	
	return glm::mat4(1.0f);
}

Frustum CLightComponent3D::GetFrustum() const
{
	Frustum frustum;
	frustum.buildViewFrustum(GetViewMatrix(), GetProjectionMatrix());
	return frustum;
}

const SLight& CLightComponent3D::GetLightStruct() const
{
	return *m_LightStruct;
}



//
// ISceneNodeComponent
//
void CLightComponent3D::Accept(IVisitor* visitor)
{
	if (GetType() == ELightType::Unknown)
		return;
	visitor->Visit((const ILight3D*) this);
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

void CLightComponent3D::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CLightComponent3D>(Destination);

	destCast->SetAmbientColor(GetAmbientColor());
	destCast->SetColor(GetColor());
	destCast->SetType(GetType());
	destCast->SetRange(GetRange());
	destCast->SetIntensity(GetIntensity());
	destCast->SetSpotlightAngle(GetSpotlightAngle());
}

void CLightComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
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

void CLightComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
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
