#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialLineProperties
	{
		SMaterialLineProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
			, Thickness(4.0f)
			, Viewport(glm::vec2(1280.0f, 1024.0f))
			, MiterLimit(0.75f)
			, SegmentsCount(30)
		{}
		glm::vec4   Color;
		float  Thickness;
		glm::vec2   Viewport;
		float  MiterLimit;
		uint32 SegmentsCount;
		//-------------------------- ( 32 bytes )
	};
}

class ZN_API UI_Line_Material 
	: public MaterialProxieT<SMaterialLineProperties>
{
public:
    UI_Line_Material(IRenderDevice& RenderDevice);
	virtual ~UI_Line_Material();

    // UI_Line_Material
	void SetColor(glm::vec4 Color);
    void SetThickness(float Thickness);
    void SetViewport(glm::vec2 Size);
    void SetMiterLimit(float MiterLimit);
    void SetSegmentsCount(uint32 SegmentsCount);
};