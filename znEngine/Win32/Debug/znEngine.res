        ��  ��                  �  ,   S H A D E R   ��f       0         #ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 1.")
#define NUM_LIGHTS 1 // should be defined by the application.
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2

struct LightMaterial
{
    float4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    float4  SpecularColor;
    //-------------------------- ( 16 bytes )
	float   SpecularPower;
	float3  Padding;
	//-------------------------- ( 16 bytes )
};

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
    * Ambient color of the light.
    */
    float4   AmbientColor;
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


float3 ExpandNormal( float3 n )
{
    return n * 2.0f - 1.0f;
}

// This lighting result is returned by the 
// lighting functions for each light type.
struct LightingResult
{
	float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

float4 DoNormalMapping( float3x3 TBN, Texture2D tex, sampler s, float2 uv )
{
    float3 normal = tex.Sample( s, uv ).xyz;
    normal = ExpandNormal( normal );

    // Transform normal from tangent space to view space.
    normal = mul( normal, TBN );
    return normalize( float4( normal, 0 ) );
}

float4 DoBumpMapping( float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale )
{
    // Sample the heightmap at the current texture coordinate.
    float height_00 = tex.Sample( s, uv ).r * bumpScale;
    // Sample the heightmap in the U texture coordinate direction.
    float height_10 = tex.Sample( s, uv, int2( 1, 0 ) ).r * bumpScale;
    // Sample the heightmap in the V texture coordinate direction.
    float height_01 = tex.Sample( s, uv, int2( 0, 1 ) ).r * bumpScale;

    float3 p_00 = { 0, 0, height_00 };
    float3 p_10 = { 1, 0, height_10 };
    float3 p_01 = { 0, 1, height_01 };

    // normal = tangent x bitangent
    float3 normal = cross( normalize(p_10 - p_00), normalize(p_01 - p_00) );

    // Transform normal from tangent space to view space.
    normal = mul( normal, TBN );

    return float4( normal, 0 );
}

float4 DoDiffuse( Light light, float4 L, float4 N )
{
    float NdotL = max( dot( N, L ), 0 );
    return light.Color * NdotL;
}

float4 DoSpecular( Light light, LightMaterial material, float4 V, float4 L, float4 N )
{
    float4 R = normalize( reflect( -L, N ) );
    float RdotV = max( dot( R, V ), 0 );

    return light.Color * pow( RdotV, material.SpecularPower );
}

// Compute the attenuation based on the range of the light.
float DoAttenuation( Light light, float d )
{
    return 1.0f - smoothstep( light.Range * 0.75f, light.Range, d );
}

float DoSpotCone( Light light, float4 L )
{
    // If the cosine angle of the light's direction 
    // vector and the vector from the light source to the point being 
    // shaded is less than minCos, then the spotlight contribution will be 0.
    float minCos = cos( radians( light.SpotlightAngle ) );
    // If the cosine angle of the light's direction vector
    // and the vector from the light source to the point being shaded
    // is greater than maxCos, then the spotlight contribution will be 1.
    float maxCos = lerp( minCos, 1, 0.5f );
    float cosAngle = dot( light.DirectionVS, -L );
    // Blend between the maxixmum and minimum cosine angles.
    return smoothstep( minCos, maxCos, cosAngle );
}

LightingResult DoPointLight( Light light, LightMaterial mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length( L );
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );

	result.Ambient = light.AmbientColor;
    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * attenuation * light.Intensity;

    return result;
}

LightingResult DoDirectionalLight( Light light, LightMaterial mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = normalize( -light.DirectionVS );

	result.Ambient = light.AmbientColor;
    result.Diffuse = DoDiffuse( light, L, N ) * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * light.Intensity;

    return result;
}

LightingResult DoSpotLight( Light light, LightMaterial mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length( L );
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    float spotIntensity = DoSpotCone( light, L );

	result.Ambient = light.AmbientColor;
    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * spotIntensity * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * attenuation * spotIntensity * light.Intensity;

    return result;
}

LightingResult DoLighting( StructuredBuffer<Light> lights, LightMaterial mat, float4 eyePos, float4 P, float4 N )
{
    float4 V = normalize( eyePos - P );

    LightingResult totalResult = (LightingResult)0;

    for ( int i = 0; i < NUM_LIGHTS; ++i )
    {
        LightingResult result = (LightingResult)0;

        // Skip lights that are not enabled.
        if ( !lights[i].Enabled ) continue;
        // Skip point and spot lights that are out of range of the point being shaded.
        if ( lights[i].Type != DIRECTIONAL_LIGHT && length( lights[i].PositionVS - P ) > lights[i].Range ) continue;

        switch ( lights[i].Type )
        {
        case DIRECTIONAL_LIGHT:
        {
            result = DoDirectionalLight( lights[i], mat, V, P, N );
        }
        break;
        case POINT_LIGHT:
        {
            result = DoPointLight( lights[i], mat, V, P, N );
        }
        break;
        case SPOT_LIGHT:
        {
            result = DoSpotLight( lights[i], mat, V, P, N );
        }
        break;
        }
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    return totalResult;
}�  ,   S H A D E R   ��g       0         #pragma once

struct PixelShaderOutput
{
	float4 PositionWS           : SV_Target0;   // Position in View space (R8G8B8_UNORM) MaterialType (A8_UNORM)            
	float4 Diffuse              : SV_Target1;   // Diffuse Albedo (R8G8B8_UNORM) Unused (A8_UNORM)
    float4 Specular             : SV_Target2;   // Specular Color (R8G8B8_UNROM) Specular Power(A8_UNORM)
    float4 NormalWS             : SV_Target3;   // View space normal (R32G32B32_FLOAT) Unused (A32_FLOAT)
};
   ,   S H A D E R   ��h       0         #include "..\\_gamedata\\shaders_D3D\\CommonInclude.hlsl"

struct VertexShaderInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

// Uniforms
cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}

cbuffer FogParams : register(b2)
{
	float  FogModifier;
	float3 FogColor;
	float  FogDistance;
	float3 Padding0;
}

cbuffer ScreenToViewParams : register( b3 ) // Parameters required to convert screen space coordinates to view space params.
{
    float4x4 InverseProjection;
    float2 ScreenDimensions;
	float2 Padding1;
	float4 CameraPos;
}

cbuffer LightIndexBuffer : register( b4 )
{
	Light light;
}

Texture2D PositionTextureWS : register( t0 ); // The position from the world space texture.
Texture2D DiffuseTextureVS  : register( t1 ); // The diffuse color from the view space texture.
Texture2D SpecularTextureVS : register( t2 ); // The specular color from the screen space texture.
Texture2D NormalTextureVS   : register( t3 ); // The normal from the screen space texture.
Texture2D DepthTextureVS    : register( t4 ); // The depth from the screen space texture.

//StructuredBuffer<Light> Lights : register(t5);

//
// HELPERS
//

// Convert clip space coordinates to view space
float4 ClipToView( float4 clip )
{
    // View space position.
    float4 view = mul( InverseProjection, clip );
    // Perspecitive projection.
    view = view / view.w;

    return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView( float4 screen )
{
    // Convert to normalized texture coordinates
    float2 texCoord = screen.xy / ScreenDimensions;

    // Convert to clip space
    float4 clip = float4( float2( texCoord.x, 1.0f - texCoord.y ) * 2.0f - 1.0f, screen.z, screen.w );

    return ClipToView( clip );
}

// Get view vector from depth
float4 GetViewFromDepth(VertexShaderOutput IN)
{
	int2 texCoord = IN.position.xy;
    float depth = DepthTextureVS.Load(int3(texCoord, 0)).r;

    // Everything is in view space.
    float4 eyePos = { 0, 0, 0, 1 };
	
	float4 P = ScreenToView(float4(texCoord, depth, 1.0f));
	
    // View vector
	return normalize( eyePos - P );
}

// Get view vector from position buffer
float4 GetViewFromPos(VertexShaderOutput IN)
{
    int2 texCoord = IN.position.xy;

    // Everything is in view space.
    float4 eyePos = CameraPos;
	
	float4 P = PositionTextureWS.Load(int3(texCoord, 0));
	
    // View vector
    return normalize( eyePos - P );
}

float4 GetFogColor(float4 P)
{
	float CameraDistance = distance(P.xyz, CameraPos.xyz);
	float CameraDistance2D = distance(P.xz, CameraPos.xz);

	float4 color = float4(0.0, 0.0, 0.0, 0.0);
	float fogStart = FogModifier * FogDistance;
	float fogFactor = (FogDistance - CameraDistance2D) / (FogDistance - fogStart);
	fogFactor = 1.0 - clamp(fogFactor, 0.0, 1.0);

	//float materialID = P.a;
	//if (materialID >= 0.0 && materialID <= 0.0)
	{
		color.rgb = FogColor * fogFactor;
		color.a = fogFactor;
	}

	// After camera end
	//if (CameraDistance2D > FogDistance) 
	//{
	//	color.rgb = FogColor;
	//	color.a = 1.0;
	//}
	
	return color;
}

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}

[earlydepthstencil]
float4 PS_DeferredLighting(VertexShaderOutput IN) : SV_Target
{
    int2 texCoord = IN.position.xy;
    float depth = DepthTextureVS.Load(int3(texCoord, 0)).r;

    // Everything is in view space.
    float4 eyePos = { 0, 0, 0, 1 };
	
	float4 P = ScreenToView(float4(texCoord, depth, 1.0f));
	
    // View vector
    float4 V = normalize( eyePos - P );

    float4 diffuse = DiffuseTextureVS.Load(int3(texCoord, 0));
    float4 specular = SpecularTextureVS.Load(int3(texCoord, 0));
    float4 N = NormalTextureVS.Load(int3(texCoord, 0));

    // Unpack the specular power from the alpha component of the specular color.
    float specularPower = exp2(specular.a);

	// If material unlit then discard
	//if (P.a == 0.0)
		return diffuse;
    
    
    LightMaterial mat = (LightMaterial)0;
    mat.DiffuseColor = diffuse;
    mat.SpecularColor = specular;
    mat.SpecularPower = specularPower;

	LightingResult lit = (LightingResult)0;
    switch ( light.Type )
    {
    case DIRECTIONAL_LIGHT:
        lit = DoDirectionalLight( light, mat, V, P, N );
        break;
    case POINT_LIGHT:
        lit = DoPointLight( light, mat, V, P, N );
        break;
    case SPOT_LIGHT:
        lit = DoSpotLight( light, mat, V, P, N );
        break;
    }
		
    return /*lit.Ambient * diffuse +*/  (diffuse * lit.Diffuse) + (specular * lit.Specular);
}

/*[earlydepthstencil]
float4 PS_DeferredLighting_FogPass(VertexShaderOutput IN) : SV_Target
{
    int2 texCoord = IN.position.xy;
	float4 P = PositionTextureWS.Load(int3(texCoord, 0));
	
	return GetFogColor(P);
}*/
   ]  ,   S H A D E R   ��i       0         #include "CommonInclude.hlsl"

VertexShaderOutput VS_main( AppData IN )
{
    VertexShaderOutput OUT;

    OUT.position = mul( ModelViewProjection, float4( IN.position, 1.0f ) );

    OUT.positionVS = mul( ModelView, float4( IN.position, 1.0f ) ).xyz;
    OUT.tangentVS = mul( ( float3x3 )ModelView, IN.tangent );
    OUT.binormalVS = mul( ( float3x3 )ModelView, IN.binormal );
    OUT.normalVS = mul( ( float3x3 )ModelView, IN.normal );

    OUT.texCoord = IN.texCoord;

    return OUT;
}

[earlydepthstencil]
float4 PS_main( VertexShaderOutput IN ) : SV_TARGET
{
    // Everything is in view space.
    float4 eyePos = { 0, 0, 0, 1 };
    Material mat = Mat;

    float4 diffuse = mat.DiffuseColor;
    if ( mat.HasDiffuseTexture )
    {
        float4 diffuseTex = DiffuseTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( diffuse.rgb ) )
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    // By default, use the alpha from the diffuse component.
    float alpha = diffuse.a;
    if ( mat.HasOpacityTexture )
    {
        // If the material has an opacity texture, use that to override the diffuse alpha.
        alpha = OpacityTexture.Sample( LinearRepeatSampler, IN.texCoord ).r;
    }

    float4 ambient = mat.AmbientColor;
    if ( mat.HasAmbientTexture )
    {
        float4 ambientTex = AmbientTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( ambient.rgb ) )
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    // Combine the global ambient term.
    ambient *= mat.GlobalAmbient;

    float4 emissive = mat.EmissiveColor;
    if ( mat.HasEmissiveTexture )
    {
        float4 emissiveTex = EmissiveTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( emissive.rgb ) )
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    if ( mat.HasSpecularPowerTexture )
    {
        mat.SpecularPower = SpecularPowerTexture.Sample( LinearRepeatSampler, IN.texCoord ).r * mat.SpecularScale;
    }

    float4 N;

    // Normal mapping
    if ( mat.HasNormalTexture )
    {
        // For scense with normal mapping, I don't have to invert the binormal.
        float3x3 TBN = float3x3( normalize( IN.tangentVS ),
                                 normalize( IN.binormalVS ),
                                 normalize( IN.normalVS ) );

        N = DoNormalMapping( TBN, NormalTexture, LinearRepeatSampler, IN.texCoord );
    }
    // Bump mapping
    else if ( mat.HasBumpTexture )
    {
        // For most scenes using bump mapping, I have to invert the binormal.
        float3x3 TBN = float3x3( normalize( IN.tangentVS ),
                                 normalize( -IN.binormalVS ), 
                                 normalize( IN.normalVS ) );

        N = DoBumpMapping( TBN, BumpTexture, LinearRepeatSampler, IN.texCoord, mat.BumpIntensity );
    }
    // Just use the normal from the model.
    else
    {
        N = normalize( float4( IN.normalVS, 0 ) );
    }

    float4 P = float4( IN.positionVS, 1 );

    LightingResult lit = DoLighting( Lights, mat, eyePos, P, N );

    diffuse *= float4( lit.Diffuse.rgb, 1.0f ); // Discard the alpha value from the lighting calculations.

    float4 specular = 0;
    if ( mat.SpecularPower > 1.0f ) // If specular power is too low, don't use it.
    {
        specular = mat.SpecularColor;
        if ( mat.HasSpecularTexture )
        {
            float4 specularTex = SpecularTexture.Sample( LinearRepeatSampler, IN.texCoord );
            if ( any( specular.rgb ) )
            {
                specular *= specularTex;
            }
            else
            {
                specular = specularTex;
            }
        }
        specular *= lit.Specular;
    }

    return float4( ( ambient + emissive + diffuse + specular ).rgb, alpha * mat.Opacity );

}

// Pixel shader for rendering lights (debug) for forward renderer.
float4 PS_light( VertexShaderOutput IN ) : SV_TARGET
{
    float4 N = normalize( float4( IN.normalVS, 0 ) );

    return float4( ( Mat.DiffuseColor * saturate(N.z) ).rgb, Mat.Opacity );
}

// Used for rendering unlit materials.
float4 PS_unlit( VertexShaderOutput IN ) : SV_Target
{
    float4 diffuse = Mat.DiffuseColor;
    if ( Mat.HasDiffuseTexture )
    {
        float4 diffuseTex = DiffuseTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( diffuse.rgb ) )
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    // By default, use the alpha from the diffuse component.
    float alpha = diffuse.a;
    if ( Mat.HasOpacityTexture )
    {
        // If the material has an opacity texture, use that to override the diffuse alpha.
        alpha = OpacityTexture.Sample( LinearRepeatSampler, IN.texCoord ).a;
    }

    if ( alpha * Mat.Opacity < Mat.AlphaThreshold )
    {
        discard;
    }

    float4 ambient = Mat.AmbientColor;
    if ( Mat.HasAmbientTexture )
    {
        float4 ambientTex = AmbientTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( ambient.rgb ) )
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    // Combine the global ambient term.
    ambient *= Mat.GlobalAmbient;

    float4 emissive = Mat.EmissiveColor;
    if ( Mat.HasEmissiveTexture )
    {
        float4 emissiveTex = EmissiveTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( emissive.rgb ) )
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    // Do view space lighting based on normal.
    float4 N = normalize( float4( IN.normalVS, 0 ) );

    return float4( ( ambient + emissive + ( diffuse * N.z ) ).rgb, alpha * Mat.Opacity );

}   �  ,   S H A D E R   ��j       0         struct Material
{
    float4  GlobalAmbient;
    //-------------------------- ( 16 bytes )
    float4  AmbientColor;
    //-------------------------- ( 16 bytes )
    float4  EmissiveColor;
    //-------------------------- ( 16 bytes )
    float4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    float4  SpecularColor;
    //-------------------------- ( 16 bytes )
    // Reflective value.
    float4  Reflectance;
    //-------------------------- ( 16 bytes )
    float   Opacity;
    float   SpecularPower;
    // For transparent materials, IOR > 0.
    float   IndexOfRefraction;
    bool    HasAmbientTexture;
    //-------------------------- ( 16 bytes )
    bool    HasEmissiveTexture;
    bool    HasDiffuseTexture;
    bool    HasSpecularTexture;
    bool    HasSpecularPowerTexture;
    //-------------------------- ( 16 bytes )
    bool    HasNormalTexture;
    bool    HasBumpTexture;
    bool    HasOpacityTexture;
    float   BumpIntensity;
    //-------------------------- ( 16 bytes )
    float   SpecularScale;
    float   AlphaThreshold;
    float2  Padding;
    //--------------------------- ( 16 bytes )
};  //--------------------------- ( 16 * 10 = 160 bytes )
   9  ,   S H A D E R   ��k       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

#define BUTTON_STATE_IDLE 0
#define BUTTON_STATE_HOVER 1
#define BUTTON_STATE_CLICKED 2
#define BUTTON_STATE_DISABLED 3

cbuffer Material : register(b1)
{
    float4 Color;
	uint   State;
};

Texture2D IdleTexture     : register(t0);
Texture2D HoverTexture    : register(t1);
Texture2D ClickedTexture  : register(t2);
Texture2D DisabledTexture : register(t3);
sampler   DiffuseTextureSampler  : register(s0);

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	
	if (State == BUTTON_STATE_IDLE)
	{
		return IdleTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_HOVER)
	{
		return HoverTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_CLICKED)
	{
		return ClickedTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	else if (State == BUTTON_STATE_DISABLED)
	{
		return DisabledTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
	}
	
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}   �   ,   S H A D E R   ��l       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer Material : register(b1)
{
    float4 Color;
};

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return Color;
}   �   ,   S H A D E R   ��m       0         struct VertexShaderInput
{
	float2 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};�  ,   S H A D E R   ��n       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
}
cbuffer Material : register(b1)
{
    float4 Color;
    float2 Offset;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	float4 resultPos = float4(IN.position, 0.0f, 1.0f) + float4(Offset, 0.0f, 0.0f);
	
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, resultPos);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return float4(Color.rgb, Color.a * DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord).a);
}   ,   S H A D E R   ��o       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

/* Geometry GLSL shader that demonstrates how to draw basic thick and smooth bezier curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * Victoria Rudakova
 * January 2017
 * MIT license
 
 * Modify: Alex Stenfard
 * March 2019
*/

struct GeometryShaderOutput
{
    float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

cbuffer Material : register(b1)
{
    float4  Color;
	float   Thickness;
	float2  Viewport;
	float   MiterLimit;
	uint    Segments;
};

static const int SegmentsMax = 30; // max_vertices = (SegmentsMax+1) * 4;
static const int SegmentsMin = 3;  // min number of segments per curve

float2 toScreenSpace(float4 vertex)
{
    return float2( vertex.xy / vertex.w ) * Viewport;
}

float toZValue(float4 vertex)
{
    return (vertex.z/vertex.w);
}

float4 toBezier(float delta, int i, float4 P0, float4 P1, float4 P2, float4 P3)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

void drawSegment(float2 points[4], float zValues[4], inout TriangleStream<GeometryShaderOutput> output)
{
    float2 p0 = points[0];
    float2 p1 = points[1];
    float2 p2 = points[2];
    float2 p3 = points[3];

    // perform naive culling
    float2 area = Viewport * 4;
    if( p1.x < -area.x || p1.x > area.x ) return;
    if( p1.y < -area.y || p1.y > area.y ) return;
    if( p2.x < -area.x || p2.x > area.x ) return;
    if( p2.y < -area.y || p2.y > area.y ) return;

    // determine the direction of each of the 3 segments (previous, current, next)
    float2 v0 = normalize( p1 - p0 );
    float2 v1 = normalize( p2 - p1 );
    float2 v2 = normalize( p3 - p2 );

    // determine the normal of each of the 3 segments (previous, current, next)
    float2 n0 = float2( -v0.y, v0.x );
    float2 n1 = float2( -v1.y, v1.x );
    float2 n2 = float2( -v2.y, v2.x );

    // determine miter lines by averaging the normals of the 2 segments
    float2 miter_a = normalize( n0 + n1 );	// miter at start of current segment
    float2 miter_b = normalize( n1 + n2 );  // miter at end of current segment

    // determine the length of the miter by projecting it onto normal and then inverse it
    float an1 = dot(miter_a, n1);
    float bn1 = dot(miter_b, n2);
    if (an1 == 0) 
		an1 = 1;
		
    if (bn1 == 0) 
		bn1 = 1;
	
    float length_a = Thickness / an1;
    float length_b = Thickness / bn1;

    // prevent excessively long miters at sharp corners
    if( dot( v0, v1 ) < (- MiterLimit) ) 
	{
        miter_a = n1;
        length_a = Thickness;

        // close the gap
        if( dot( v0, n1 ) > 0 ) 
		{
            //VertexOut.mColor = colors[1];
			GeometryShaderOutput o;
            o.position = float4( ( p1 + Thickness * n0 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0 );
			output.Append(o);

            //VertexOut.mColor = colors[1];
            o.position = float4( ( p1 + Thickness * n1 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0 );
			output.Append(o);
			
            //VertexOut.mColor = colors[1];
            o.position = float4( p1 / Viewport, 0.0, 1.0 );
			o.texCoord = float2( 0, 0.5 );
			output.Append(o);
			
			output.RestartStrip();
        }
        else 
		{
            //colors[1];
			GeometryShaderOutput o;
            o.position = float4( ( p1 - Thickness * n1 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 1 );
			output.Append(o);

            //colors[1];
            o.position = float4( ( p1 - Thickness * n0 ) / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 1 );
			output.Append(o);

            //colors[1];
            o.position = float4( p1 / Viewport, zValues[1], 1.0 );
			o.texCoord = float2( 0, 0.5 );
			output.Append(o);
			
			output.RestartStrip();
        }
    }
	
    if( dot( v1, v2 ) < -MiterLimit ) 
	{
        miter_b = n1;
        length_b = Thickness;
    }
	
	GeometryShaderOutput o;
    o.position = float4( ( p1 + length_a * miter_a ) / Viewport, zValues[1], 1.0 );
	o.texCoord = float2( 0, 0 );
	output.Append(o);

    o.position = float4( ( p1 - length_a * miter_a ) / Viewport, zValues[1], 1.0 );
	o.texCoord = float2( 0, 1 );
	output.Append(o);

    o.position = float4( ( p2 + length_b * miter_b ) / Viewport, zValues[2], 1.0 );
	o.texCoord = float2( 0, 0 );
	output.Append(o);

    o.position = float4( ( p2 - length_b * miter_b ) / Viewport, zValues[2], 1.0 );
	o.texCoord = float2( 0, 1 );
	output.Append(o);

	output.RestartStrip();
}

[maxvertexcount(124)]
void GS_main(lineadj VertexShaderOutput points[4], inout TriangleStream<GeometryShaderOutput> output)
{
    // cut segments number if larger or smaller than allowed
    uint nSegments = (Segments > SegmentsMax)? SegmentsMax : Segments;
    nSegments = (nSegments < SegmentsMin)? SegmentsMin: nSegments;

    // 4 control points
    float4 B[4] = { points[0].position, points[1].position, points[2].position, points[3].position };

    // use the points to build a bezier line
    float delta = 1.0 / float(nSegments);
	
    float4 Points[4] = { float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 0.0f) }; // segments of curve in 3d
    float zValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    	
    for (uint i = 0; i <= nSegments; i++)
	{
        // first point
        if (i == 0)
		{
            Points[1] = toBezier(delta, i, B[0], B[1], B[2], B[3]);
            Points[2] = toBezier(delta, i+1, B[0], B[1], B[2], B[3]);
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
            float4 dir = normalize(Points[1] - Points[2]);
            Points[0] = Points[1] + dir * 0.01;
        }
        else if (i < nSegments - 1)
		{
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            Points[3] = toBezier(delta, i+2, B[0], B[1], B[2], B[3]);
        }
        // last point
        else 
		{
            Points[0] = Points[1];
            Points[1] = Points[2];
            Points[2] = Points[3];
            float4 dir = normalize(Points[2] - Points[1]);
            Points[3] = Points[2] + dir * 0.001f;
        }

        // segments of curve in 2d
        float2 points[4];
        points[0] = toScreenSpace(Points[0]);
        points[1] = toScreenSpace(Points[1]);
        points[2] = toScreenSpace(Points[2]);
        points[3] = toScreenSpace(Points[3]);

        zValues[0] = toZValue(Points[0]);
        zValues[1] = toZValue(Points[1]);
        zValues[2] = toZValue(Points[2]);
        zValues[3] = toZValue(Points[3]);

        drawSegment(points, zValues, output);
    }
}

float4 PS_LineV2(GeometryShaderOutput input) : SV_TARGET
{
    return Color;
}
 T  ,   S H A D E R   ��p       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer Material : register(b1)
{
    float4 Color;
};

Texture2D DiffuseTexture : register(t0);
sampler DiffuseTextureSampler : register(s0);

float4 PS_main(VertexShaderOutput IN) : SV_TARGET
{
	return DiffuseTexture.Sample(DiffuseTextureSampler, IN.texCoord) * Color;
}u  ,   S H A D E R   ��q       0         #include "..\\_gamedata\\shaders_D3D\\UI\UI_Common.h"

cbuffer PerObject : register(b0)
{
	float4x4 ModelViewProjection;
	float4x4 ModelView;
	float4x4 Model;
}

VertexShaderOutput VS_main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.position = mul(ModelViewProjection, float4(IN.position, 0.0f, 1.0f));
	OUT.texCoord = IN.texCoord;
	return OUT;
}
   