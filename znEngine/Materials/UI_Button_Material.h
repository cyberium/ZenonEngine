#pragma once

class ZN_API UI_Button_Material : public MaterialProxie
{
	typedef MaterialProxie base;
public:
	UI_Button_Material(IRenderDevice* RenderDevice);
	virtual ~UI_Button_Material();

	void SetIdleTexture(ITexture* _texture);
	void SetHoverTexture(ITexture* _texture);
	void SetClickedTexture(ITexture* _texture);
	void SetDisabledTexture(ITexture* _texture);

	void SetColor(vec4 color);
	void SetState(uint32 state);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		vec4   Color;
		uint32 State;
		vec3   Padding;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};