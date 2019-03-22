#pragma once

#include "../../MaterialWrapper.h"

class UI_Texture_Material : public MaterialWrapper
{
	typedef MaterialWrapper base;
public:
	UI_Texture_Material();
	virtual ~UI_Texture_Material();

	void SetTexture(std::shared_ptr<Texture> _texture);
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