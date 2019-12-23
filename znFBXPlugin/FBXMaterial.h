#pragma once

class CFBXMaterial
	: public MaterialProxie
{
public:
	CFBXMaterial(std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~CFBXMaterial();

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
	MaterialProperties*            m_pProperties;
private:
	std::shared_ptr<IRenderDevice> m_RenderDevice;
};