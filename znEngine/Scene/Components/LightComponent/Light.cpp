#include "stdafx.h"

// General
#include "Light.h"

CLight::CLight(const IBaseManager& BaseManager)
	: Object(BaseManager)
	, m_IsEnabled(true)
	, m_IsCastShadows(false)
{
	//m_GPULightStruct = (SGPULight*)_aligned_malloc(sizeof(SGPULight), 16);
	//*m_GPULightStruct = SGPULight();
}

CLight::~CLight()
{
	//if (m_GPULightStruct != nullptr)
	//	_aligned_free(m_GPULightStruct);
}



//
// ILight
//
void CLight::SetEnabled(bool Value)
{
	m_IsEnabled = Value;
}

bool CLight::IsEnabled() const
{
	return m_IsEnabled;
}

void CLight::SetType(ELightType Value)
{
	m_Type = Value;
}

ELightType CLight::GetType() const
{
	return m_Type;
}

SGPULight CLight::GetGPULightStruct() const
{
	SGPULight gpuLightStruct;
	gpuLightStruct.AmbientColor = ColorRGBA(m_AmbientColor, 1.0f);
	gpuLightStruct.Color = ColorRGBA(m_Color, 1.0f);
	gpuLightStruct.Range = m_Range;
	gpuLightStruct.Intensity = m_Intensity;
	gpuLightStruct.SpotlightAngle = m_SpotlightAngle;

	return gpuLightStruct;
}

// Static

void CLight::SetAmbientColor(ColorRGB Value)
{
	m_AmbientColor = Value;
}

ColorRGB CLight::GetAmbientColor() const
{
	return m_AmbientColor;
}

void CLight::SetColor(ColorRGB Value)
{
	m_Color = Value;
}

ColorRGB CLight::GetColor() const
{
	return m_Color;
}

void CLight::SetRange(float Value)
{
	m_Range = Value;
}

float CLight::GetRange() const
{
	return m_Range;
}

void CLight::SetIntensity(float Value)
{
	m_Intensity = Value;
}

float CLight::GetIntensity() const
{
	return m_Intensity;
}

void CLight::SetSpotlightAngle(float Value)
{
	m_SpotlightAngle = Value;
}

float CLight::GetSpotlightAngle() const
{
	return m_SpotlightAngle;
}

// Shadows

void CLight::SetCastShadows(bool Value)
{
	m_IsCastShadows = Value;
}

bool CLight::IsCastShadows() const
{
	return m_IsCastShadows;
}

glm::mat4 CLight::GetViewMatrix() const
{
	if (m_Type == ELightType::Point)
	{
		throw CException("Not implemented.");
	}
	else if (m_Type == ELightType::Spot)
	{
		return glm::lookAt(m_Position, m_Position + m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (m_Type == ELightType::Directional)
	{
		return glm::lookAt(- glm::normalize(m_Rotation) * 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	throw CException("Unknown light type '%d'.", m_Type);
}

glm::mat4 CLight::GetProjectionMatrix() const
{
	if (m_Type == ELightType::Point)
	{
		throw CException("Not implemented.");
	}
	else if (m_Type == ELightType::Spot)
	{
		return glm::perspectiveRH_ZO(glm::radians(m_SpotlightAngle), 1.0f, 1.0f, m_Range);
	}
	else if (m_Type == ELightType::Directional)
	{
		const float t = 100.0f; // TODO: Fixme
		return glm::orthoRH_ZO<float>(-t, t, -t, t, -200.0f, 200.0f);
	}

	throw CException("Unknown light type '%d'.", m_Type);
}

Frustum CLight::GetFrustum() const
{
	Frustum frustum;
	frustum.buildViewFrustum(GetViewMatrix(), GetProjectionMatrix());
	return frustum;
}



//
// ILightInternal
//
void CLight::SetPosition(const glm::vec3& Position)
{
	m_Position = Position;
}

void CLight::SetRotation(const glm::vec3& Rotation)
{
	m_Rotation = Rotation;
}



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



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CLight::Copy() const
{
	return std::shared_ptr<IObject>();
}

void CLight::CopyTo(std::shared_ptr<IObject> Destination) const
{
	auto destCast = std::dynamic_pointer_cast<ILight>(Destination);

	destCast->SetEnabled(IsEnabled());
	destCast->SetCastShadows(IsCastShadows());

	destCast->SetType(GetType());

	destCast->SetAmbientColor(GetAmbientColor());
	destCast->SetColor(GetColor());
	destCast->SetRange(GetRange());
	destCast->SetIntensity(GetIntensity());
	destCast->SetSpotlightAngle(GetSpotlightAngle());
}

void CLight::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	throw CException("Not impleemented.");
}

void CLight::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	throw CException("Not impleemented.");
}

void CLight::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	auto isEnabled = Reader->GetChild("IsEnabled");
	if (isEnabled == nullptr)
		throw CException("Unable to find 'IsEnabled' xml child.");
	SetEnabled(isEnabled->GetBool());

	auto isCastShadows = Reader->GetChild("IsCastShadows");
	if (isCastShadows == nullptr)
		throw CException("Unable to find 'IsCastShadows' xml child.");
	SetCastShadows(isCastShadows->GetBool());

	auto type = Reader->GetChild("Type");
	if (type == nullptr)
		throw CException("Unable to find 'Type' xml child.");
	SetType(LightTypeFromString(type->GetValue()));

	auto ambientColor = Reader->GetChild("AmbientColor");
	if (ambientColor == nullptr)
		throw CException("Unable to find 'AmbientColor' xml child.");
	SetAmbientColor(ambientColor->GetColorRGB());

	auto color = Reader->GetChild("Color");
	if (color == nullptr)
		throw CException("Unable to find 'Color' xml child.");
	SetColor(color->GetColorRGB());

	auto range = Reader->GetChild("Range");
	if (range == nullptr)
		throw CException("Unable to find 'Range' xml child.");
	SetRange(range->GetFloat());

	auto intensity = Reader->GetChild("Intensity");
	if (intensity == nullptr)
		throw CException("Unable to find 'Intensity' xml child.");
	SetIntensity(intensity->GetFloat());

	auto spotlightAngle = Reader->GetChild("SpotlightAngle");
	if (spotlightAngle == nullptr)
		throw CException("Unable to find 'SpotlightAngle' xml child.");
	SetSpotlightAngle(spotlightAngle->GetFloat());
}

void CLight::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	auto enabled = Writer->CreateChild("Enabled");
	enabled->SetBool(IsEnabled());

	auto isCastShadows = Writer->CreateChild("CastShadows");
	isCastShadows->SetBool(IsCastShadows());

	auto type = Writer->CreateChild("Type");
	type->SetValue(LightTypeToString(GetType()));

	auto ambientColor = Writer->CreateChild("AmbientColor");
	ambientColor->SetColorRGB(GetAmbientColor());

	auto color = Writer->CreateChild("Color");
	color->SetColorRGB(GetColor());

	auto range = Writer->CreateChild("Range");
	range->SetFloat(GetRange());

	auto intensity = Writer->CreateChild("Intensity");
	intensity->SetFloat(GetIntensity());

	auto spotlightAngle = Writer->CreateChild("SpotlightAngle");
	spotlightAngle->SetFloat(GetSpotlightAngle());
}
