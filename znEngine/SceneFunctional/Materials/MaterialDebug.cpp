#include "stdafx.h"

// Include
#include "MaterialWrapper.h"

// General
#include "MaterialDebug.h"

// Additional
#include <Application.h>

MaterialDebug::MaterialDebug(std::shared_ptr<Material> _material)
	: MaterialWrapper(_material)
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	MaterialWrapper::CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));

	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Debug/Debug.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	g_pVertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Debug/Debug.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	SetShader(Shader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(Shader::ShaderType::PixelShader, g_pPixelShader);
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
