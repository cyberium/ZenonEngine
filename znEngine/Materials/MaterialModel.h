#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialModelProperties
	{
		SMaterialModelProperties()
			: Ambient(glm::vec3(0.0f))
			, AmbientFactor(0.0f)

			, Diffuse(glm::vec3(1.0f))
			, DiffuseFactor(1.0f)

			, Specular(glm::vec3(1.0f))
			, SpecularFactor(1.0f)

			, Emissive(glm::vec3(0.0f))
			, EmissiveFactor(1.0f)

			, Reflection(glm::vec3(0.0f))
			, ReflectionFactor(1.0f)

			, HasTextureAmbient(false)
			, HasTextureDiffuse(false)
			, HasTextureSpecular(false)
			, HasTextureEmissive(false)
			
			, HasTextureShininess(false)
			, HasTextureNormalMap(false)
			, HasTextureBump(false)
			, HasTextureTransparency(false)

			, HasTextureReflection(false)
			, HasTextureDisplacement(false)
			, Shininess(12.0f)
			, BumpFactor(1.0f)
		{}

		glm::vec3 Ambient;					// Ambient color property.
		float AmbientFactor;				// Ambient factor property. This factor is used to attenuate the ambient color.

		glm::vec3 Diffuse;					// Diffuse color property.
		float DiffuseFactor;				// Diffuse factor property. This factor is used to attenuate the diffuse color.

		glm::vec3 Specular;					// Specular property.
		float SpecularFactor;				// Specular factor property. This factor is used to  attenuate the specular color.

		glm::vec3 Emissive;					// Emissive color property.
		float EmissiveFactor;				// Emissive factor property. This factor is used to attenuate the emissive color.

		glm::vec3 Reflection;				// Reflection color property. This property is used to implement reflection mapping.
		float ReflectionFactor;				// Reflection factor property. This property is used to attenuate the reflection color.

		uint32 HasTextureAmbient;
		uint32 HasTextureDiffuse;
		uint32 HasTextureSpecular;
		uint32 HasTextureEmissive;
		
		uint32 HasTextureShininess;
		uint32 HasTextureNormalMap;
		uint32 HasTextureBump;
		uint32 HasTextureTransparency;

		uint32 HasTextureReflection;
		uint32 HasTextureDisplacement;
		float Shininess;                    // Shininess property. This property controls the aspect of the shiny spot. It is the specular exponent in the Phong illumination model.
		float BumpFactor;					// Bump factor property. This factor is used to make a surface more or less bumpy.
	};
}

class ZN_API MaterialModel
	: public MaterialProxieT<SMaterialModelProperties>
{
public:
	enum ZN_API ETextureType: uint32
	{
		TextureAmbient       = 0,
		TextureDiffuse       = 1,
		TextureSpecular      = 2,
		TextureEmissive      = 3,

		TextureShininess     = 4,
		TextureNormalMap     = 5,
		TextureBump          = 6,
		TextureTransparency  = 7,

		TextureReflection    = 8,
		TextureDisplacement  = 9,
		
		Count
	};

public:
	MaterialModel(const IBaseManager& BaseManage);
	virtual ~MaterialModel();

	void SetAmbientColor(glm::vec3 Color);
	glm::vec3 GetAmbientColor() const;

	void SetAmbientFactor(float Factor);

	void SetDiffuseColor(glm::vec3 Color);
	glm::vec3 GetDiffuseColor() const;

	void SetDiffuseFactor(float Factor);

	void SetSpecularColor(glm::vec3 Color);
	glm::vec3 GetSpecularColor() const;
	
	void SetSpecularFactor(float Factor);

	void SetEmissiveColor(glm::vec3 Color);
	glm::vec3 GetEmissiveColor() const;

	void SetEmissiveFactor(float Factor);

	void SetShininess(float Factor);
	void SetBumpFactor(float Factor);

	void SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture);
	std::shared_ptr<ITexture> GetTexture(ETextureType TextureType) const;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;
	
protected:
	void PrintInfo();
	std::string GetTextureTypeNameByTextureID(ETextureType ID) const;
	ETextureType GetTextureIDByTextureTypeName(const char* TypeName) const;

protected:
	const IBaseManager& m_BaseManager;
};