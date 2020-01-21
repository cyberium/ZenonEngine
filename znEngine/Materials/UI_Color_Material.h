#pragma once

class ZN_API UI_Color_Material : public MaterialProxie
{
public:
	UI_Color_Material(IRenderDevice* RenderDevice);
	virtual ~UI_Color_Material();

	void SetColor(vec4 color);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1, 1, 1, 1)
		{}
		vec4 Color;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};