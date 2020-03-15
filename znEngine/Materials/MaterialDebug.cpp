#include "stdafx.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(IRenderDevice& RenderDevice)
	: MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;
	if (RenderDevice.GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		vertexShader = RenderDevice.GetObjectsFactory().CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", "VS_main", IShader::ShaderMacros(),  "latest"
		);
		pixelShader = RenderDevice.GetObjectsFactory().CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", "PS_main", IShader::ShaderMacros(),  "latest"
		);
	}
	else
	{
		vertexShader = RenderDevice.GetObjectsFactory().CreateShader(
			EShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", "", IShader::ShaderMacros(),  ""
		);
		pixelShader = RenderDevice.GetObjectsFactory().CreateShader(
			EShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", "", IShader::ShaderMacros(),  ""
		);
	}
	vertexShader->LoadInputLayoutFromReflector();

	//SetShader(EShaderType::VertexShader, vertexShader);
	//SetShader(EShaderType::PixelShader, pixelShader);
}

MaterialDebug::~MaterialDebug()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

const glm::vec4& MaterialDebug::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

//-----

void MaterialDebug::SetDiffuseColor(const glm::vec4& diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialDebug::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
