#pragma once

class OW_ENGINE_API COBJMaterial : public MaterialProxie
{
public:
	COBJMaterial(std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~COBJMaterial();

	void UpdateConstantBuffer() const override;


	/*
		Textures:
		0 - Ambient
		1 - Diffuse
		2 - Specular Texture
		3 - Specular Hightlight Map
		4 - Alpha Texture Map
		5 - Bump Map
	*/


	inline void Set_Ka(glm::vec3 Ka)
	{
		m_pProperties->Ka = Ka;
	}

	inline void Set_Kd(glm::vec3 Kd)
	{
		m_pProperties->Kd = Kd;
	}

	inline void Set_Ks(glm::vec3 Ks)
	{
		m_pProperties->Ks = Ks;
	}


	inline void Set_Ns(float Ns)
	{
		m_pProperties->Ns = Ns;
	}

	inline void Set_Ni(float Ni)
	{
		m_pProperties->Ni = Ni;
	}

	inline void Set_d(float d)
	{
		m_pProperties->d = d;
	}

	inline void Set_illum(int illum)
	{
		m_pProperties->illum = illum;
	}

	//
	// Textures
	//

	inline void Set_AmbientTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(0, texture);
		m_pProperties->HasAmbientTexture = true;
		UpdateConstantBuffer();
	}
	inline void Set_DiffuseTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(1, texture);
		m_pProperties->HasDiffuseTexture = true;
		UpdateConstantBuffer();
	}
	inline void Set_SpecularTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(2, texture);
		m_pProperties->HasSpecularTexture = true;
		UpdateConstantBuffer();
	}
	inline void Set_SpecularHightlightTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(3, texture);
		m_pProperties->HasSpecularHightlightTexture = true;
		UpdateConstantBuffer();
	}
	inline void Set_AlphaTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(4, texture);
		m_pProperties->HasAlphaTexture = true;
		UpdateConstantBuffer();
	}
	inline void Set_BumpTexture(std::shared_ptr<ITexture> texture)
	{
		_ASSERT(texture != nullptr);
		SetTexture(5, texture);
		m_pProperties->HasBumpTexture = true;
		UpdateConstantBuffer();
	}

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
		{
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;

			HasAmbientTexture = false;
			HasDiffuseTexture = false;
			HasSpecularTexture = false;
			HasSpecularHightlightTexture = false;
			HasAlphaTexture = false;
			HasBumpTexture = false;
		}

		// Ambient Color
		glm::vec3 Ka;
		float _padd0;
		// 16 bytes

		// Diffuse Color
		glm::vec3 Kd;
		float _padd1;
		// 16 bytes

		// Specular Color
		glm::vec3 Ks;
		float _padd2;
		// 4 bytes

		// Specular Exponent
		float Ns;
		// Optical Density
		float Ni;
		// Dissolve
		float d;
		// Illumination
		int illum;
		// 16 bytes

		int HasAmbientTexture;
		int HasDiffuseTexture;
		int HasSpecularTexture;
		int HasSpecularHightlightTexture;
		// 16 bytes

		int HasAlphaTexture;
		int HasBumpTexture;
		float _padd3;
		float _padd4;
		// 16 bytes
	};
	MaterialProperties* m_pProperties;
};