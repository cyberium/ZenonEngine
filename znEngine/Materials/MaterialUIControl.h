#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialUIControlProperties
	{
		SMaterialUIControlProperties()
			: DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
			, HasTextureDiffuse(false)
		{}
		glm::vec4 DiffuseColor;
		//-------------------------- ( 16 bytes )
		uint32    HasTextureDiffuse;
		glm::vec3 _padding;
		//-------------------------- ( 16 bytes )
	};
}

class ZN_API CMaterialUIControl
	: public MaterialProxieT<SMaterialUIControlProperties>
{
public:
	CMaterialUIControl(IRenderDevice& RenderDevice);
	virtual ~CMaterialUIControl();

	// CMaterialUIControl
	void SetColor(glm::vec4 color);
	glm::vec4 GetColor() const;
	void SetTexture(std::shared_ptr<ITexture> Texture);
	std::shared_ptr<ITexture> GetTexture() const;
};