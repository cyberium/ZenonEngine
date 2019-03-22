#pragma once

#include "../../MaterialWrapper.h"

class UI_Line_Material : public MaterialWrapper
{
public:
    UI_Line_Material();
	virtual ~UI_Line_Material();

    // UI_Line_Material
	void SetColor(vec4 Color);
    void SetThickness(float Thickness);
    void SetViewport(vec2 Size);
    void SetMiterLimit(float MiterLimit);
    void SetSegmentsCount(uint32 SegmentsCount);

protected:
	void UpdateConstantBuffer() const override;

private:
	__declspec(align(16)) struct MaterialProperties
	{
		MaterialProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
            , Thickness(4.0f)
            , Viewport(vec2(1280.0f, 1024.0f))
            , MiterLimit(0.75f)
            , SegmentsCount(30)
		{}
		vec4   Color;
        float  Thickness;
        vec2   Viewport;
        float  MiterLimit;
        uint32 SegmentsCount;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties* m_pProperties;
};