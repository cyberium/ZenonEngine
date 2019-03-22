#pragma once

#include "Light.h"

class CLight3D : public Object
{
public:
	CLight3D(const Light& _light);
	virtual ~CLight3D();

	void setLight(const Light& _light);
	const Light& getLight();

	bool Accept(IVisitor& visitor);

private:
	const Light& m_LightProto;
};