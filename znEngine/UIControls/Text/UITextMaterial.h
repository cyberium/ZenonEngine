#pragma once

struct __declspec(align(16)) SGPUPerCharacterDataPS
{
	SGPUPerCharacterDataPS()
		: Color(1.0f, 1.0f, 1.0f, 1.0f)
		, IsSelected(0)
	{}
	ColorRGBA Color;
	// 16 bytes

	uint32 IsSelected;
	glm::vec3 Padding;
	// 16 bytes
};

class CUITextMaterial
	: public MaterialProxieT<SGPUPerCharacterDataPS>
{
public:
	CUITextMaterial(const IRenderDevice& RenderDevice);
	virtual ~CUITextMaterial();

	void SetTexture(const std::shared_ptr<ITexture>& Texture);
	std::shared_ptr<ITexture> GetTexture() const;

	void SetColor(const ColorRGBA& Color);
	ColorRGBA GetColor() const;
};