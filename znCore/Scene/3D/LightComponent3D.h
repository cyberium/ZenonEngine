#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_LightComponent) ZN_API CLightComponent3D
	: public ILightComponent3D
	, public CComponentBase
{
public:
    CLightComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CLightComponent3D();

	// ILightComponent3D
	void SetColor(glm::vec3 Value) override;
	glm::vec3 GetColor() const override;
	void SetType(ELightType Value) override;
	ELightType GetType() const override;
	void SetRange(float Value) override;
	float GetRange() const override;
	void SetIntensity(float Value) override;
	float GetIntensity() const override;
	void SetSpotlightAngle(float Value) override;
	float GetSpotlightAngle() const override;

	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjectionMatrix() const override;

	const SLight& GetLightStruct() const override;

    // ISceneNodeComponent
	virtual void DoUpdate(UpdateEventArgs& e) override;
    virtual bool Accept(IVisitor* visitor) override;

private:
	SLight* m_LightStruct;
};