#pragma once

class ZN_API UI_Font_Material : public MaterialProxie
{
public:
	UI_Font_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Font_Material();

	// UI_Font_Material
	void SetColor(glm::vec4 color);
	void SetOffset(glm::vec2 offset);
	void SetSelected(bool Value);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
			, Offset(0.0f, 0.0f)
			, IsSelected(false)
		{}
		glm::vec4 Color;
		glm::vec2 Offset;
		uint32 IsSelected;
		float Padding;
	};
	MaterialProperties* m_pProperties;
};