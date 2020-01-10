#pragma once

class ZN_API MaterialModel
	: public MaterialProxie
{
public:
	enum class ZN_API ETextureType
	{
		TextureDiffuse = 0,
		TextureEmissive,
		TextureAmbient,
		TextureSpecular,
		TextureShininess,
		TextureNormalMap,
		TextureBump,
		TextureTransparency,
		TextureReflection,
		TextureDisplacement
	};

public:
	MaterialModel(const IBaseManager* BaseManage);
	virtual ~MaterialModel();

	void SetEmissiveColor(const glm::vec3& Color);
	void SetAmbientColor(const glm::vec3& Color);
	void SetDiffuseColor(const glm::vec3& Color);

	void SetBump(const glm::vec3& Color);
	void SetBumpFactor(float Factor);

	void SetSpecularColor(const glm::vec3& Color);
	void SetSpecularFactor(float Factor);

	void SetReflectionColor(const glm::vec3& Color);
	void SetReflectionFactor(float Factor);

	virtual std::shared_ptr<ITexture> GetTexture(ETextureType TextureType) const;
	virtual void SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture);

protected:
	void UpdateConstantBuffer() const override;
	void PrintInfo();

protected:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Emissive(glm::vec3(0.0f, 0.0f, 0.0f))
			, EmissiveFactor(1.0f)

			, Ambient(glm::vec3(0.0f, 0.0f, 0.0f))
			, AmbientFactor(1.0f)

			, Diffuse(glm::vec3(1.0f, 1.0f, 1.0f))
			, DiffuseFactor(0.8f)

			, NormalMap(glm::vec3(0.0f, 0.0f, 0.0f))
			, Shininess(12.0f)

			, Bump(glm::vec3(0.0f, 0.0f, 0.0f))
			, BumpFactor(1.0f)

			, TransparentColor(glm::vec3(0.0f, 0.0f, 0.0f))
			, TransparencyFactor(1.0f)

			, DisplacementColor(glm::vec3(0.0f, 0.0f, 0.0f))
			, DisplacementFactor(0.5f)

			, VectorDisplacementColor(glm::vec3(0.0f, 0.0f, 0.0f))
			, VectorDisplacementFactor(0.5f)

			, Specular(glm::vec3(0.0f, 0.0f, 0.0f))
			, SpecularFactor(1.0f)

			, Reflection(glm::vec3(0.0f, 0.0f, 0.0f))
			, ReflectionFactor(1.0f)

			, HasTextureDiffuse(false)
			, HasTextureEmissive(false)
			, HasTextureAmbient(false)
			, HasTextureSpecular(false)

			, HasTextureShininess(false)
			, HasTextureNormalMap(false)
			, HasTextureBump(false)
			, HasTextureTransparency(false)

			, HasTextureReflection(false)
			, HasTextureDisplacement(false)
		{}

		glm::vec3 Emissive;					// Emissive color property.
		float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.

		glm::vec3 Ambient;					// Ambient color property.
		float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.

		glm::vec3 Diffuse;					// Diffuse color property.
		float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.

		glm::vec3 NormalMap;				// NormalMap property. This property can be used to specify the distortion of the surface normals and create the illusion of a bumpy surface.
		float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.

		glm::vec3 Bump;						// Bump property. This property is used to distort the surface normal and create the illusion of a bumpy surface.
		float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.

		glm::vec3 TransparentColor;			// Transparent color property.
		float TransparencyFactor;			// Transparency factor property. This property is used to make a surface more or less opaque (0 = opaque, 1 = transparent).

		glm::vec3 DisplacementColor;		// Displacement color property.
		float DisplacementFactor;			// Displacement factor property.

		glm::vec3 VectorDisplacementColor;	// Vector displacement color property.
		float VectorDisplacementFactor;		// Vector displacement factor property.

		glm::vec3 Specular;					// Specular property.
		float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.

		glm::vec3 Reflection;				// Reflection color property. This property is used to implement reflection mapping.
		float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.

		uint32 HasTextureDiffuse;
		uint32 HasTextureEmissive;
		uint32 HasTextureAmbient;
		uint32 HasTextureSpecular;

		uint32 HasTextureShininess;
		uint32 HasTextureNormalMap;
		uint32 HasTextureBump;
		uint32 HasTextureTransparency;

		uint32 HasTextureReflection;
		uint32 HasTextureDisplacement;
		glm::vec2 Padding;
	};
	MaterialProperties* m_pProperties;

protected:
	const IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_RenderDevice;
};