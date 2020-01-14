#include "stdafx.h"

// General
#include "Light3D.h"

CLight3D::CLight3D()
{
	m_LightProto = (Light*)_aligned_malloc(sizeof(Light), 16);
	*m_LightProto = Light();
}

CLight3D::CLight3D(Light _light)
{
	m_LightProto = (Light*)_aligned_malloc(sizeof(Light), 16);
	*m_LightProto = Light();

	setLight(_light);
}

CLight3D::~CLight3D()
{
	if (m_LightProto)
	{
		_aligned_free(m_LightProto);
		m_LightProto = nullptr;
	}
}

void CLight3D::setLight(const Light& _light)
{
	*m_LightProto = _light;
}

Light& CLight3D::getLight()
{
	return *m_LightProto;
}

glm::mat4 CLight3D::GetViewMatrix() const
{
	return glm::lookAt(m_LightProto->m_PositionWS.xyz(), m_LightProto->m_PositionWS.xyz() + m_LightProto->m_DirectionWS.xyz(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 CLight3D::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_LightProto->m_SpotlightAngle * 2.0f), 1.0f, 0.5f, 10000.0f);
}


bool CLight3D::Accept(IVisitor* visitor)
{
	if (m_LightProto->m_Enabled)
		return visitor->Visit(this);

	return false;
}
