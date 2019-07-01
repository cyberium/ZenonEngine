#include <stdafx.h>

// General
#include "UI_Font_Material.h"

// Additional
#include "Application.h"

UI_Font_Material::UI_Font_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial())
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();
	CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));

	// CreateShaders

    std::shared_ptr<Shader> g_pVertexShader;
	std::shared_ptr<Shader> g_pPixelShader;

    if (_RenderDevice->GetDeviceType() == IRenderDevice::DeviceType::DirectX)
    {
        g_pVertexShader = _RenderDevice->CreateShader(
            Shader::VertexShader, "IDB_SHADER_UI_FONT", Shader::ShaderMacros(), "VS_main", "latest"
        );
        
        g_pPixelShader = _RenderDevice->CreateShader(
            Shader::PixelShader, "IDB_SHADER_UI_FONT", Shader::ShaderMacros(), "PS_main", "latest"
        );
    }
    else if (_RenderDevice->GetDeviceType() == IRenderDevice::DeviceType::OpenGL)
    {
        g_pVertexShader = _RenderDevice->CreateShader(
            Shader::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", Shader::ShaderMacros(), "VS_main", "latest"
        );

        g_pPixelShader = _RenderDevice->CreateShader(
            Shader::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", Shader::ShaderMacros(), "PS_main", "latest"
        );
    }

    g_pVertexShader->LoadInputLayoutFromReflector();

	// Create samplers
	std::shared_ptr<SamplerState> g_LinearClampSampler = _RenderDevice->CreateSamplerState();
	g_LinearClampSampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp, SamplerState::WrapMode::Clamp);

    SetSampler(0, g_LinearClampSampler);

	// Material
	SetShader(Shader::VertexShader, g_pVertexShader);
	SetShader(Shader::PixelShader, g_pPixelShader);
}

UI_Font_Material::~UI_Font_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}



//
// UI_Font_Material
//
void UI_Font_Material::SetColor(vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}

void UI_Font_Material::SetOffset(vec2 offset)
{
	m_pProperties->Offset = offset;
	MarkConstantBufferDirty();
}

void UI_Font_Material::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
