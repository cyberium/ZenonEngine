struct MaterialModel
{
	float3 Emissive;					// Emissive color property.
	float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.
	// 16

	float3 Ambient;					    // Ambient color property.
	float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.
	// 16

	float3 Diffuse;					    // Diffuse color property.
	float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.
	// 16

	float3 NormalMap;				    // NormalMap property. This property can be used to specify the distortion of the surface normals and create the illusion of a bumpy surface.
	float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.
	// 16

	float3 Bump;						// Bump property. This property is used to distort the surface normal and create the illusion of a bumpy surface.
	float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.
	// 16

	float3 TransparentColor;			// Transparent color property.
	float TransparencyFactor;			// Transparency factor property. This property is used to make a surface more or less opaque (0 = opaque, 1 = transparent).
	// 16

	float3 DisplacementColor;           // Displacement color property.
	float DisplacementFactor;			// Displacement factor property.
	// 16

	float3 VectorDisplacementColor;     // Vector displacement color property.
	float VectorDisplacementFactor;		// Vector displacement factor property.
	// 16

	float3 Specular;					// Specular property.
	float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.
	// 16

	float3 Reflection;				    // Reflection color property. This property is used to implement reflection mapping.
	float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.
	// 16
	// 160

	bool HasTextureDiffuse;
	bool HasTextureEmissive;
	bool HasTextureAmbient;
	bool HasTextureSpecular;
	// 16

	bool HasTextureShininess;
	bool HasTextureNormalMap;
	bool HasTextureBump;
	bool HasTextureTransparency;
	// 16

	bool HasTextureReflection;
	bool HasTextureDisplacement;
	float2 Padding;
};
