#pragma once

// FORWARD BEGIN

// FORWARD END


// DON'T FORGET ABOUT PADDINGS!
// DON'T USE 'bool'!


enum class ZN_API ELightType : uint32 // Don't delete uint32_t becouse mapped to render
{
	Point = 0,
	Spot,
	Directional
};


struct __declspec(align(16)) ZN_API SGPULight
{
	SGPULight()
		: Type(ELightType::Point)
		, Range(1000.0f)
		, Intensity(1.0f)
		, SpotlightAngle(45.0f)
		, AmbientColor(0.1f, 0.1f, 0.1f, 1.0f)
		, Color(1.0f, 1.0f, 1.0f, 1.0f)
	{}
	ELightType Type; // The type of the light.
	float Range; // The range of the light.
	float Intensity; // The intensity of the light.
	float SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------(16 bytes )
	ColorRGBA AmbientColor;// Ambient color of the light.
	//--------------------------------------------------------------(16 bytes )
	ColorRGBA Color;       // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------(16 bytes )

};


ZN_INTERFACE ZN_API ILight
	: public virtual IObject
{
	virtual ~ILight() {}

	virtual void SetEnabled(bool Value) = 0;
	virtual bool IsEnabled() const = 0;
	virtual void SetType(ELightType Value) = 0;
	virtual ELightType GetType() const = 0;
	virtual SGPULight GetGPULightStruct() const = 0;

	// Static params
	virtual void SetAmbientColor(ColorRGB Value) = 0;
	virtual ColorRGB GetAmbientColor() const = 0;
	virtual void SetColor(ColorRGB Value) = 0;
	virtual ColorRGB GetColor() const = 0;
	virtual void SetRange(float Value) = 0;
	virtual float GetRange() const = 0;
	virtual void SetIntensity(float Value) = 0;
	virtual float GetIntensity() const = 0;
	virtual void SetSpotlightAngle(float Value) = 0;
	virtual float GetSpotlightAngle() const = 0;

	// Shadows 
	virtual void SetCastShadows(bool Value) = 0;
	virtual bool IsCastShadows() const = 0;
	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;
	virtual Frustum GetFrustum() const = 0;
};

ZN_INTERFACE ZN_API ILightInternal
{
	virtual ~ILightInternal() {}

	virtual void SetPosition(const glm::vec3& Position) = 0;
	virtual void SetRotation(const glm::vec3& Rotation) = 0;
};

ZN_INTERFACE ZN_API ILightComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeLightComponent);

	virtual ~ILightComponent3D() {}

	virtual void SetLight(std::shared_ptr<ILight> Light) = 0;
	virtual std::shared_ptr<ILight> GetLight() const = 0;
};
