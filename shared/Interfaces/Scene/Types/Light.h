#pragma once

// DON'T FORGET ABOUT PADDINGS!
// DON'T USE 'bool'!

enum class ZN_API ELightType : uint32_t //Don't delete uint32_t becouse mapped to render
{
	Point = 0,
	Spot,
	Directional,
	Unknown
};

struct __declspec(novtable, align(16)) ZN_API SLight
{
	SLight::SLight()
		: PositionWS(0, 0, 0, 1)
		, DirectionWS(0, 0, -1, 0)
		, PositionVS(0, 0, 0, 1)
		, DirectionVS(0, 0, 1, 0)
		, Color(1, 1, 1, 1)

		, Enabled(false)
		, Range(99999.0f)
		, Intensity(1.0f)
		, SpotlightAngle(45.0f)

		, Type(ELightType::Unknown)
	{}

	// Position for point and spot lights (World space).
	glm::vec4 PositionWS;
	//--------------------------------------------------------------( 16 bytes )
	// Direction for spot and directional lights (World space).
	glm::vec4 DirectionWS;
	//--------------------------------------------------------------( 16 bytes )
	// Position for point and spot lights (View space).
	glm::vec4 PositionVS;
	//--------------------------------------------------------------( 16 bytes )
	// Direction for spot and directional lights (View space).
	glm::vec4 DirectionVS;
	//--------------------------------------------------------------( 16 bytes )
	// Color of the light. Diffuse and specular colors are not separated.
	glm::vec4 Color;
	//--------------------------------------------------------------( 16 bytes )
	// Disable or enable the light.
	uint32 Enabled;
	// The range of the light.
	float Range;
	// The intensity of the light.
	float Intensity;
	// The half angle of the spotlight cone.
	float SpotlightAngle;
	//--------------------------------------------------------------(16 bytes )
	// The type of the light.
	ELightType Type;
	glm::vec3 __Padding;
	//--------------------------------------------------------------(16 bytes )
	//--------------------------------------------------------------( 16 * 7 = 112 bytes )
};
