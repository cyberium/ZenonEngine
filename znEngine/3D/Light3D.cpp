#include <stdafx.h>

// General
#include "Light3D.h"

CLight3D::CLight3D(const Light& _light)
	: m_LightProto(_light)
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::setLight(const Light& _light)
{
	//m_LightProto = _light;
}

const Light& CLight3D::getLight()
{
	return m_LightProto;
}


bool CLight3D::Accept(IVisitor& visitor)
{
	if (m_LightProto.m_Enabled)
		return visitor.Visit(*this);

	return false;
}
