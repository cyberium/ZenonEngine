#pragma once

#include "UI_Color_Material.h"

class ZN_API UI_Texture_Material 
	: public UI_Color_Material
{
public:
	UI_Texture_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Texture_Material();

	void SetTexture(const std::shared_ptr<ITexture> _texture);
};