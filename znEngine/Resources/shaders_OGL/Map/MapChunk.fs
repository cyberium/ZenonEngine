#version 440
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_enhanced_layouts : enable
#extension GL_ARB_separate_shader_objects : enable

// In put
in struct
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
} VSInput;

// Uniforms
uniform Material 
{
    vec4 DiffuseColor;
	uint gLayersCount;
	vec3 pad;
};

// Layers
//uniform bool gIsNortrend;
//uniform bool gIsMCCVExists;

layout(location = 0) uniform sampler2D gColorMap[4];
layout(location = 4) uniform sampler2D gBlend;
layout(location = 5) uniform sampler2D gSpecularMap[4];

layout(location = 0) out vec4 color;

void main()
{
	vec3 layersColor = vec3(0);
	vec4 layersSpecular = vec4(0);
	vec3 resultColor = vec3(0);
	vec4 resultSpecular = vec4(0);

	/*if (gIsNortrend)
	{
		float alphaSumma = 0.0;
		for(uint i = 1u; i < gLayersCount; i++)
		{
			float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha)[i - 1u];
			alphaSumma += alphaCurrent;
			layersColor += texture(gColorMap[i], VSInput.TexCoordDetail).rgb * alphaCurrent;
			layersSpecular += texture(gSpecularMap[i], VSInput.TexCoordDetail) * alphaCurrent;
		}

		resultColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb * (1.0 - alphaSumma) + layersColor;
		resultSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail) * (1.0 - alphaSumma) + layersSpecular;
	}
	else*/
	{
		layersColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb;
		layersSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail);

		for(uint i = 1u; i < gLayersCount; i++)
		{
			float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha)[i - 1u];
			layersColor = mix(layersColor, texture(gColorMap[i], VSInput.TexCoordDetail).rgb, alphaCurrent);
			layersSpecular = mix(layersSpecular, texture(gSpecularMap[i], VSInput.TexCoordDetail),alphaCurrent);
		}

		resultColor = layersColor;
		resultSpecular = layersSpecular;
	}
			
	//if (gShadowMapExists)
	//{
	//	float alphaShadow = texture(gBlend, VSInput.TexCoordAlpha).a;
	//	resultColor = mix(resultColor,  gShadowColor, alphaShadow);
	//}

	//if (gIsMCCVExists)
	//{
	//	resultColor *= (VSInput.VertexColorMCCV.rgb * 2.0f);
	//}

	//if (gIsLowRes == 2) // Low
	//{
	//	resultColor *= vec3(1.0f, 0.7f, 0.7f);
	//}
	//else if (gIsLowRes == 1) // Default
	//{
	//	resultColor *= vec3(0.8f, 1.0f, 0.8f);
	//}

	//
	
	color = vec4(resultColor.rgb, 1.0f);
};
