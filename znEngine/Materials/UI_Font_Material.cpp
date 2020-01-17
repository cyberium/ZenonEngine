#include "stdafx.h"

// General
#include "UI_Font_Material.h"

UI_Font_Material::UI_Font_Material(std::shared_ptr<IRenderDevice> RenderDevice) :
	MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
    std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

    if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
    {
        g_pVertexShader = RenderDevice->CreateShader(
            EShaderType::VertexShader, "IDB_SHADER_UI_FONT", IShader::ShaderMacros(), "VS_main", "latest"
        );
        
        g_pPixelShader = RenderDevice->CreateShader(
            EShaderType::PixelShader, "IDB_SHADER_UI_FONT", IShader::ShaderMacros(), "PS_main", "latest"
        );
    }
    else if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
    {
        g_pVertexShader = RenderDevice->CreateShader(
            EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", IShader::ShaderMacros(), "", ""
        );

        g_pPixelShader = RenderDevice->CreateShader(
            EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", IShader::ShaderMacros(), "", ""
        );
    }

    g_pVertexShader->LoadInputLayoutFromReflector();

	// Create samplers
	std::shared_ptr<ISamplerState> g_LinearClampSampler = RenderDevice->CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

    SetSampler(0, g_LinearClampSampler);

	// Material
	SetShader(EShaderType::VertexShader, g_pVertexShader);
	SetShader(EShaderType::PixelShader, g_pPixelShader);
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
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
