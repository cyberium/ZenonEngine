#pragma once

// FORWARD BEGIN
class MaterialWrapper;
// FORWARD END

class MaterialDebug : public MaterialWrapper
{
public:
	MaterialDebug(std::shared_ptr<Material> _material);
	virtual ~MaterialDebug();

	cvec4 GetDiffuseColor() const;
	void SetDiffuseColor(cvec4 diffuse);

	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1, 1, 1, 1)
		{}
		vec4   m_DiffuseColor;
		//-------------------------- ( 16 bytes )
	};
	MaterialProperties* m_pProperties;
};