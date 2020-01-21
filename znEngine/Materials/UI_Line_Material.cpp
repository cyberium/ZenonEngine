#include "stdafx.h"

// General
#include "UI_Line_Material.h"

UI_Line_Material::UI_Line_Material(IRenderDevice* RenderDevice) :
	MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	IShader* g_pVertexShader = RenderDevice->CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_LINE", IShader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

    IShader* g_pGeomShader = RenderDevice->CreateShader(
        EShaderType::GeometryShader, "IDB_SHADER_UI_LINE", IShader::ShaderMacros(), "GS_main", "latest"
    );

	IShader* g_pPixelShader = RenderDevice->CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_LINE", IShader::ShaderMacros(), "PS_LineV2", "latest"
	);

	// Material
	SetShader(EShaderType::VertexShader, g_pVertexShader);
    SetShader(EShaderType::GeometryShader, g_pGeomShader);
	SetShader(EShaderType::PixelShader, g_pPixelShader);
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
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
