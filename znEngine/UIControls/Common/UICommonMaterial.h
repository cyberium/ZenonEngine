#pragma once

namespace
{
	struct __declspec(align(16)) SUIControlCommonMaterialData
	{
		SUIControlCommonMaterialData()
			: DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f)
			, HasTextureDiffuse(false)
		{}
		ColorRGBA DiffuseColor;
		//-------------------------- ( 16 bytes )
		uint32    HasTextureDiffuse;
		glm::vec3 _padding;
		//-------------------------- ( 16 bytes )
	};
}

class ZN_API CUICommonMaterial
	: public MaterialProxieT<SUIControlCommonMaterialData>
	, public IUIControlCommonMaterial
{
public:
	CUICommonMaterial(IRenderDevice& RenderDevice);
	virtual ~CUICommonMaterial();

	// IMaterialUIControl
	void SetColor(ColorRGBA color) override;
	ColorRGBA GetColor() const override;
	void SetTexture(std::shared_ptr<ITexture> Texture) override;
	std::shared_ptr<ITexture> GetTexture() const override;
};