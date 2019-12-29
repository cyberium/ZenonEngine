#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct Light
{
	/**
	* Position for point and spot lights (World space).
	*/
	float4   PositionWS;
	//--------------------------------------------------------------( 16 bytes )
	/**
	* Direction for spot and directional lights (World space).
	*/
	float4   DirectionWS;
	//--------------------------------------------------------------( 16 bytes )
	/**
	* Position for point and spot lights (View space).
	*/
	float4   PositionVS;
	//--------------------------------------------------------------( 16 bytes )
	/**
	* Direction for spot and directional lights (View space).
	*/
	float4   DirectionVS;
	//--------------------------------------------------------------( 16 bytes )
	/**
	* Color of the light. Diffuse and specular colors are not seperated.
	*/
	float4   Color;
	//--------------------------------------------------------------( 16 bytes )
	/**
	* The half angle of the spotlight cone.
	*/
	float    SpotlightAngle;
	/**
	* The range of the light.
	*/
	float    Range;

	/**
	 * The intensity of the light.
	 */
	float    Intensity;

	/**
	* Disable or enable the light.
	*/
	bool    Enabled;
	//--------------------------------------------------------------( 16 bytes )

	/**
	 * Is the light selected in the editor?
	 */
	bool    Selected;

	/**
	* The type of the light.
	*/
	uint    Type;
	float2  Padding;
	//--------------------------------------------------------------( 16 bytes )
	//--------------------------------------------------------------( 16 * 7 = 112 bytes )
};