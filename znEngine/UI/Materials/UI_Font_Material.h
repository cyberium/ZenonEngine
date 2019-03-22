#pragma once

#include "../../MaterialWrapper.h"

class UI_Font_Material : public MaterialWrapper
{
public:
	UI_Font_Material();
	virtual ~UI_Font_Material();

	// UI_Font_Material
	void SetColor(glm::vec4 color);
	void SetOffset(glm::vec2 offset);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
			, Offset(0.0f, 0.0f)
		{}
		glm::vec4 Color;
		glm::vec2 Offset;
		glm::vec2 Padding;
	};
	MaterialProperties* m_pProperties;
};