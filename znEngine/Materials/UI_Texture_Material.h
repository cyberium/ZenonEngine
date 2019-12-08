#pragma once

class OW_ENGINE_API UI_Texture_Material : public MaterialProxie
{
	typedef MaterialProxie base;
public:
	UI_Texture_Material(std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~UI_Texture_Material();

	void SetTexture(std::shared_ptr<ITexture> _texture);
	void SetColor(vec4 color);
	
protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		vec4 Color;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};