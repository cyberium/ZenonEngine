#pragma once

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXSceneNode;
// FORWARD END

class ZN_API CFBXMaterial
	: public MaterialProxie
{
public:
	CFBXMaterial(const IBaseManager* BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode, fbxsdk::FbxSurfaceMaterial* NativeMaterial);
	virtual ~CFBXMaterial();

	void Load();

protected:
	inline float ToFloat(const FbxPropertyT<FbxDouble>& FBXDouble)
	{
		return FBXDouble.Get();
	}
	inline glm::vec3 ToGLMVec3(const FbxPropertyT<FbxDouble3>& FBXVec3)
	{
		return glm::vec3(FBXVec3.Get()[0], FBXVec3.Get()[1], FBXVec3.Get()[2]);
	}

	std::shared_ptr<ITexture> LoadTexture(fbxsdk::FbxTexture* Texture);

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: HasTextureDiffuse(false)
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
	MaterialProperties* m_pProperties;

protected:
	void UpdateConstantBuffer() const override;

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;
	fbxsdk::FbxSurfaceMaterial* m_NativeMaterial;

private:
	const IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_RenderDevice;
};