#include "stdafx.h"

// General
#include "UI_Line_Material.h"

UI_Line_Material::UI_Line_Material(IRenderDevice& RenderDevice) :
	MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	const auto& g_pVertexShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_LINE", "VS_main", IShader::ShaderMacros(), "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	const auto& g_pGeomShader = RenderDevice.GetObjectsFactory().CreateShader(
        EShaderType::GeometryShader, "IDB_SHADER_UI_LINE", "GS_main", IShader::ShaderMacros(), "latest"
    );

	const auto& g_pPixelShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_LINE", "PS_LineV2", IShader::ShaderMacros(), "latest"
	);

	// Material
	//SetShader(EShaderType::VertexShader, g_pVertexShader);
    //SetShader(EShaderType::GeometryShader, g_pGeomShader);
	//SetShader(EShaderType::PixelShader, g_pPixelShader);
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
