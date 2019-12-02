#include "stdafx.h"

// General
#include "MaterialDebug.h"

// Additional
#include <Application.h>

MaterialDebug::MaterialDebug()
	: MaterialWrapper(_RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;
	if (_RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = _RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "VS_main", "latest"
		);
		g_pPixelShader = _RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "PS_main", "latest"
		);
	}
	else
	{
		g_pVertexShader = _RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", IShader::ShaderMacros(), "", ""
		);
		g_pPixelShader = _RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", IShader::ShaderMacros(), "", ""
		);
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);
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
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
