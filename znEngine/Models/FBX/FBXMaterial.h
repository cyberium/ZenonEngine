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

	inline void SetAmbient(vec3 Value) 
	{ 
		m_pProperties->Ambient = Value; 
	}
	inline void SetDiffuse(vec3 Value)
	{ 
		m_pProperties->Diffuse = Value; 
	}
	inline void SetSpecular(vec3 Value)
	{ 
		m_pProperties->Specular = Value; 
	}
	inline void SetEmissive(vec3 Value)
	{ 
		m_pProperties->Emissive = Value; 
	}

	inline void SetTransparencyFactor(float Value) 
	{ 
		m_pProperties->TransparencyFactor = Value; 
	}
	inline void SetShininess(float Value) 
	{ 
		m_pProperties->Shininess = Value; 
	}
	inline void SetReflectionFactor(float Value)
	{ 
		m_pProperties->ReflectionFactor = Value; 
	}

protected:
	std::shared_ptr<ITexture> LoadTexture(fbxsdk::FbxTexture* Texture);

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
		{}
		vec3   Ambient;
		vec3   Diffuse;
		vec3   Specular;
		vec3   Emissive;
		float  TransparencyFactor;
		float  Shininess;
		float  ReflectionFactor;
	};
	MaterialProperties* m_pProperties;

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;
	fbxsdk::FbxSurfaceMaterial* m_NativeMaterial;

private:
	const IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_RenderDevice;
};