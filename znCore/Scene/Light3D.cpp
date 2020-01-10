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


bool CLight3D::Accept(IVisitor* visitor)
{
	if (m_LightProto->m_Enabled)
		return visitor->Visit(this);

	return false;
}
