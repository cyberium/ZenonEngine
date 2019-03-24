#pragma once

#include "Light.h"

class CLight3D : public Object
{
public:
	CLight3D(Light _light);
	virtual ~CLight3D();

	void setLight(const Light& _light);
	Light& getLight();

	bool Accept(IVisitor& visitor);

private:
	Light m_LightProto;
};