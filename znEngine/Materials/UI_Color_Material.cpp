#include "stdafx.h"

// General
#include "UI_Color_Material.h"

UI_Color_Material::UI_Color_Material(std::shared_ptr<IRenderDevice> RenderDevice) :
	MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	std::shared_ptr<IShader> g_pVertexShader = RenderDevice->CreateShader(
		SShaderType::VertexShader, "IDB_SHADER_UI_COLOR", IShader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> g_pPixelShader = RenderDevice->CreateShader(
		SShaderType::PixelShader, "IDB_SHADER_UI_COLOR", IShader::ShaderMacros(), "PS_main", "latest"
	);

	// Material
	SetShader(SShaderType::VertexShader, g_pVertexShader);
	SetShader(SShaderType::PixelShader, g_pPixelShader);
}

UI_Color_Material::~UI_Color_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Color_Material::SetColor(vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}

void UI_Color_Material::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
