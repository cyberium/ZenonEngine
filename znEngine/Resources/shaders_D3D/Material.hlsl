struct Material
{
	float3 Emissive;					// Emissive color property.
	float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.

	float3 Ambient;					    // Ambient color property.
	float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.

	float3 Diffuse;					    // Diffuse color property.
	float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.

	float3 NormalMap;				    // NormalMap property. This property can be used to specify the distortion of the surface normals and create the illusion of a bumpy surface.
	float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.

	float3 Bump;						// Bump property. This property is used to distort the surface normal and create the illusion of a bumpy surface.
	float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.

	float3 TransparentColor;			// Transparent color property.
	float TransparencyFactor;			// Transparency factor property. This property is used to make a surface more or less opaque (0 = opaque, 1 = transparent).

	float3 DisplacementColor;           // Displacement color property.
	float DisplacementFactor;			// Displacement factor property.

	float3 VectorDisplacementColor;     // Vector displacement color property.
	float VectorDisplacementFactor;		// Vector displacement factor property.

	float3 Specular;					// Specular property.
	float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.

	float3 Reflection;				    // Reflection color property. This property is used to implement reflection mapping.
	float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.

	bool HasTextureDiffuse;
	bool HasTextureEmissive;
	bool HasTextureAmbient;
	bool HasTextureSpecular;

	bool HasTextureShininess;
	bool HasTextureNormalMap;
	bool HasTextureBump;
	bool HasTextureTransparency;

	bool HasTextureReflection;
	bool HasTextureDisplacement;
};
