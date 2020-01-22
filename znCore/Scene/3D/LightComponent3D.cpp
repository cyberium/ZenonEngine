#include "stdafx.h"

// General
#include "LightComponent3D.h"

CLightComponent3D::CLightComponent3D(std::shared_ptr<ISceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
{
	m_LightStruct = (SLight*)_aligned_malloc(sizeof(SLight), 16);
	*m_LightStruct = SLight();
}

CLightComponent3D::~CLightComponent3D()
{
	_aligned_free(m_LightStruct);
}



//
// ILightComponent3D
//
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

glm::mat4 CLightComponent3D::GetViewMatrix() const
{
	if (m_LightStruct->Type != ELightType::Unknown)
	{
		if (m_LightStruct->Type == ELightType::Directional)
		{
			glm::vec3 position = -m_LightStruct->DirectionWS.xyz();
			position *= 100.0f;
			return glm::lookAt(position, position + m_LightStruct->DirectionWS.xyz(), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (m_LightStruct->Type == ELightType::Spot)
		{
			return glm::lookAt(m_LightStruct->PositionWS.xyz(), m_LightStruct->PositionWS.xyz() + m_LightStruct->DirectionWS.xyz(), glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	return glm::mat4(1.0f);
}

glm::mat4 CLightComponent3D::GetProjectionMatrix() const
{
	if (m_LightStruct->Type != ELightType::Unknown)
	{
		if (m_LightStruct->Type == ELightType::Directional)
		{
			const float t = 300.0f;
			return glm::ortho<float>(-t, t, -t, t, -t, t);
		}
		else if (m_LightStruct->Type == ELightType::Spot)
		{
			return glm::perspective(glm::radians(m_LightStruct->SpotlightAngle * 2.0f), 1.0f, 0.5f, 10000.0f);
		}
	}
	
	return glm::mat4(1.0f);
}

const SLight& CLightComponent3D::GetLightStruct() const
{
	return *m_LightStruct;
}



//
// ISceneNodeComponent
//
void CLightComponent3D::DoUpdate(UpdateEventArgs & e)
{
	m_LightStruct->PositionWS = glm::vec4(std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode())->GetTranslation(), 1.0f);
	m_LightStruct->DirectionWS = glm::vec4(std::dynamic_pointer_cast<ISceneNode3D>(GetOwnerNode())->GetRotation(), 0.0f);
}

bool CLightComponent3D::Accept(IVisitor* visitor)
{
	if (GetType() != ELightType::Unknown)
		return visitor->Visit(this);

	return false;
}
