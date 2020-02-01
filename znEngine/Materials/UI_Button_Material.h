#pragma once

class ZN_API UI_Button_Material : public MaterialProxie
{
	typedef MaterialProxie base;
public:
	UI_Button_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Button_Material();

	void SetIdleTexture(std::shared_ptr<ITexture> _texture);
	void SetHoverTexture(std::shared_ptr<ITexture> _texture);
	void SetClickedTexture(std::shared_ptr<ITexture> _texture);
	void SetDisabledTexture(std::shared_ptr<ITexture> _texture);

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