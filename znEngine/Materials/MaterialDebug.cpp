#include "stdafx.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(IRenderDevice* RenderDevice)
	: MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	IShader* g_pVertexShader;
	IShader* g_pPixelShader;
	if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = RenderDevice->CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "VS_main", "latest"
		);
		g_pPixelShader = RenderDevice->CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "PS_main", "latest"
		);
	}
	else
	{
		g_pVertexShader = RenderDevice->CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", IShader::ShaderMacros(), "", ""
		);
		g_pPixelShader = RenderDevice->CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", IShader::ShaderMacros(), "", ""
		);
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	SetShader(EShaderType::VertexShader, g_pVertexShader);
	SetShader(EShaderType::PixelShader, g_pPixelShader);
}

MaterialDebug::~MaterialDebug()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

cvec4 MaterialDebug::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

//-----

void MaterialDebug::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialDebug::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
