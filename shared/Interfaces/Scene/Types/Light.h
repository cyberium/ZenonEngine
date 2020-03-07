#pragma once

// DON'T FORGET ABOUT PADDINGS!
// DON'T USE 'bool'!

enum class ZN_API ELightType : uint32_t // Don't delete uint32_t becouse mapped to render
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
		, DirectionWS(0, -1, 0, 0)
		, PositionVS(0, 0, 0, 1)
		, DirectionVS(0, 0, 1, 0)
		, Color(1.0f, 1.0f, 1.0f, 1.0f)

		, Type(ELightType::Unknown)
		, Range(99999.0f)
		, Intensity(1.0f)
		, SpotlightAngle(45.0f)
	{}

	
	glm::vec4 PositionWS;  // Position for point and spot lights (World space).
	//--------------------------------------------------------------( 16 bytes )
	glm::vec4 DirectionWS; // Direction for spot and directional lights (World space).
	//--------------------------------------------------------------( 16 bytes )
	glm::vec4 PositionVS;  // Position for point and spot lights (View space).
	//--------------------------------------------------------------( 16 bytes )
	glm::vec4 DirectionVS; // Direction for spot and directional lights (View space).
	//--------------------------------------------------------------( 16 bytes )
	glm::vec4 Color;       // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------( 16 bytes )
	
	ELightType Type; // The type of the light.
	float Range; // The range of the light.
	float Intensity; // The intensity of the light.
	float SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------(16 bytes )

	//--------------------------------------------------------------( 16 * 7 = 112 bytes )
};
