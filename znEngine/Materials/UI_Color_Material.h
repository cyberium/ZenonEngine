#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialUIColorProperties
	{
		SMaterialUIColorProperties()
			: Color(1, 1, 1, 1)
		{}
		glm::vec4 Color;
		//-------------------------- ( 32 bytes )
	};
}

class ZN_API UI_Color_Material 
	: public MaterialProxieT<SMaterialUIColorProperties>
{
public:
	UI_Color_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Color_Material();

	void SetColor(glm::vec4 color);
};