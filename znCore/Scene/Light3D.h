#pragma once

#include "Light.h"

class ZN_API CLight3D 
	: public Object
	, public std::enable_shared_from_this<CLight3D>
{
public:
	CLight3D();
	CLight3D(Light _light);
	virtual ~CLight3D();

	void setLight(const Light& _light);
	Light& getLight();

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	bool Accept(IVisitor* visitor);

private:
	Light* m_LightProto;
};