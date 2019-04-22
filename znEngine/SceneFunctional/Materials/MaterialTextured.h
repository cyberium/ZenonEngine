#pragma once

// FORWARD BEGIN
class MaterialWrapper;
// FORWARD END

class MaterialTextured : public MaterialWrapper
{
public:
	MaterialTextured(std::shared_ptr<Material> _material);
	virtual ~MaterialTextured();

	void SetTexture(std::shared_ptr<Texture> texture);
	void SetNormalTexture(std::shared_ptr<Texture> texture);
	void SetHeightTexture(std::shared_ptr<Texture> texture);

protected:
	void UpdateConstantBuffer() const;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_HasNormalTexture(0)
			, m_HasHeightTexture(0)
		{}
		uint32 m_HasNormalTexture;
		uint32 m_HasHeightTexture;
		vec2   Padding0;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties*            m_pProperties;
};