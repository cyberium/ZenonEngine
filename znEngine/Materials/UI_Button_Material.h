#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialUIButtonProperties
	{
		SMaterialUIButtonProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		glm::vec4 Color;
		uint32 State;
		glm::vec3 Padding;
		//-------------------------- ( 32 bytes )
	};
}

class ZN_API UI_Button_Material 
	: public MaterialProxieT<SMaterialUIButtonProperties>
{
public:
	UI_Button_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Button_Material();

	void SetIdleTexture(std::shared_ptr<ITexture> _texture);
	void SetHoverTexture(std::shared_ptr<ITexture> _texture);
	void SetClickedTexture(std::shared_ptr<ITexture> _texture);
	void SetDisabledTexture(std::shared_ptr<ITexture> _texture);

	void SetColor(glm::vec4 color);
	void SetState(uint32 state);
};