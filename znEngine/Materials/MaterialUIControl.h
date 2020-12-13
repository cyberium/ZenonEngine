#pragma once

namespace
{
	struct __declspec(align(16)) SMaterialUIControlProperties
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
	, public IMaterialUIControl
{
public:
	CMaterialUIControl(IRenderDevice& RenderDevice);
	virtual ~CMaterialUIControl();

	// IMaterialUIControl
	void SetColor(glm::vec4 color) override;
	glm::vec4 GetColor() const override;
	void SetTexture(std::shared_ptr<ITexture> Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;
};