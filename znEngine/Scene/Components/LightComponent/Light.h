#pragma once

#include "../Animations/AnimatedValueSingle.h"

class ZN_API CLight
	: public ILight
	, public ILightInternal
	, public IObjectLoadSave
{
public:
	CLight();
	virtual ~CLight();

	// ILight
	void SetEnabled(bool Value) override;
	bool IsEnabled() const override;
	void SetType(ELightType Value) override;
	ELightType GetType() const override;
	SGPULight GetGPULightStruct() const override;

	// Static
	void SetAmbientColor(glm::vec3 Value) override;
	glm::vec3 GetAmbientColor() const override;
	void SetColor(glm::vec3 Value) override;
	glm::vec3 GetColor() const override;
	void SetRange(float Value) override;
	float GetRange() const override;
	void SetIntensity(float Value) override;
	float GetIntensity() const override;
	void SetSpotlightAngle(float Value) override;
	float GetSpotlightAngle() const override;

	// Shadows
	void SetCastShadows(bool Value) override;
	bool IsCastShadows() const override;
	glm::mat4 GetViewMatrix() const override;
	glm::mat4 GetProjectionMatrix() const override;
	Frustum GetFrustum() const override;

	// ILightInternal
	void SetPosition(const glm::vec3& Position) override;
	void SetRotation(const glm::vec3& Rotation) override;

	// IObjectLoadSave
	std::shared_ptr<IObject> Copy() const override;
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;
	 
private:
	bool m_IsEnabled;
	ELightType m_Type;
	bool m_IsCastShadows;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	
	// Static values
	glm::vec3 m_AmbientColor;
	glm::vec3 m_Color;
	float m_Range;
	float m_Intensity;
	float m_SpotlightAngle;

	// Animated values
	CznAnimatedValue<glm::vec3> m_AmbientAnimated;
	CznAnimatedValue<glm::vec3> m_ColorAnimated;
	CznAnimatedValue<float> m_RangeAnimated;
	CznAnimatedValue<float> m_IntensityAnimated;
	CznAnimatedValue<float> m_SpotlightAngleAnimated;
};