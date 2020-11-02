#pragma once

class __declspec(UUID_LightComponent) ZN_API CLightComponent3D
	: public ILightComponent3D
	, public ILight3D
	, public CComponentBase
{
public:
    CLightComponent3D(const ISceneNode& OwnerNode);
    virtual ~CLightComponent3D();

	// ILightComponent3D
	void SetAmbientColor(glm::vec3 Value) override;
	glm::vec3 GetAmbientColor() const override;
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

	// ILight3D
	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjectionMatrix() const override;
	const SLight& GetLightStruct() const override;

    // ISceneNodeComponent
	virtual void Update(const UpdateEventArgs& e) override;
    virtual void Accept(IVisitor* visitor) override;

private:
	SLight* m_LightStruct;
};