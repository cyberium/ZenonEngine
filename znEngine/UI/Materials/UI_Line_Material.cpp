#include "stdafx.h"

// General
#include "UI_Line_Material.h"

// Additional
#include "Application.h"

UI_Line_Material::UI_Line_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial())
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));

	// CreateShaders
	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/UI/UI_VertexBase.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

    std::shared_ptr<Shader> g_pGeomShader = _RenderDevice->CreateShader(
        Shader::GeometryShader, "shaders_D3D/UI/UI_Line.hlsl", Shader::ShaderMacros(), "GS_main", "latest"
    );

	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/UI/UI_Line.hlsl", Shader::ShaderMacros(), "PS_LineV2", "latest"
	);

	// Material
	SetShader(Shader::VertexShader, g_pVertexShader);
    SetShader(Shader::GeometryShader, g_pGeomShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

UI_Line_Material::~UI_Line_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}



//
// UI_Line_Material
//
void UI_Line_Material::SetColor(vec4 Color)
{
	m_pProperties->Color = Color;
	MarkConstantBufferDirty();
}

void UI_Line_Material::SetThickness(float Thickness)
{
    m_pProperties->Thickness = Thickness;
    MarkConstantBufferDirty();
}

void UI_Line_Material::SetViewport(vec2 Size)
{
    m_pProperties->Viewport = Size;
    MarkConstantBufferDirty();
}

void UI_Line_Material::SetMiterLimit(float MiterLimit)
{
    m_pProperties->MiterLimit = MiterLimit;
    MarkConstantBufferDirty();
}

void UI_Line_Material::SetSegmentsCount(uint32 SegmentsCount)
{
    m_pProperties->SegmentsCount = SegmentsCount;
    MarkConstantBufferDirty();
}

void UI_Line_Material::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
