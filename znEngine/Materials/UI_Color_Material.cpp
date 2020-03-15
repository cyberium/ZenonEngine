#include "stdafx.h"

// General
#include "UI_Color_Material.h"

UI_Color_Material::UI_Color_Material(IRenderDevice& RenderDevice) :
	MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	const auto& g_pVertexShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_COLOR", "VS_main", IShader::ShaderMacros(), "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	const auto& g_pPixelShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_COLOR", "PS_main", IShader::ShaderMacros(), "latest"
	);

	// Material
	//SetShader(EShaderType::VertexShader, g_pVertexShader);
	//SetShader(EShaderType::PixelShader, g_pPixelShader);
}

UI_Color_Material::~UI_Color_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Color_Material::SetColor(glm::vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}

void UI_Color_Material::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
