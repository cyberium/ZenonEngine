layout(location = 0) out vec4 fragData0;
layout(location = 1) out vec4 fragData1;
layout(location = 2) out vec4 fragData2;
layout(location = 3) out vec4 fragData3;

void setMatID(const float id) 
{ 
	fragData0.a = id; 
}
void setPos(const vec3 pos) 
{ 
	fragData0.rgb = pos; 
}
void setNormal(const vec3 normal) 
{ 
	fragData1.rgb = normal; 
	fragData1.a = 1.0; 
}
void setAlbedo(const vec3 albedo) 
{ 
	fragData2.rgb = albedo; 
	fragData2.a = 1.0; 
}
void setAlbedo4(const vec4 albedo) 
{ 
	fragData2 = albedo; 
}
void setSpecParams(const vec3 specCol, const float gloss) 
{ 
	fragData3.rgb = specCol; 
	fragData3.a = gloss; 
}

struct MaterialType
{
    vec4  GlobalAmbient;
    //-------------------------- ( 16 bytes )
    vec4  AmbientColor;
    //-------------------------- ( 16 bytes )
    vec4  EmissiveColor;
    //-------------------------- ( 16 bytes )
    vec4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    vec4  SpecularColor;
    //-------------------------- ( 16 bytes )
    // Reflective value.
    vec4  Reflectance;
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
    vec2  Padding;
    //--------------------------- ( 16 bytes )
};  //--------------------------- ( 16 * 10 = 160 bytes )


layout(location = 0) uniform sampler2D AmbientTexture;
layout(location = 1) uniform sampler2D EmissiveTexture;
layout(location = 2) uniform sampler2D DiffuseTexture;
layout(location = 3) uniform sampler2D SpecularTexture;
layout(location = 4) uniform sampler2D SpecularPowerTexture;
layout(location = 5) uniform sampler2D NormalTexture;
layout(location = 6) uniform sampler2D BumpTexture;
layout(location = 7) uniform sampler2D OpacityTexture;
