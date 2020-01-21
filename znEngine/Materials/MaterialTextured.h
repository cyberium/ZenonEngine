#pragma once

class ZN_API MaterialTextured : public MaterialProxie
{
public:
	MaterialTextured(IRenderDevice* RenderDevice);
	virtual ~MaterialTextured();

	cvec4 GetDiffuseColor() const;
	void SetDiffuseColor(cvec4 diffuse);

	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: m_DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		vec4   m_DiffuseColor;
		//-------------------------- ( 4 bytes )
	};
	MaterialProperties*            m_pProperties;
};