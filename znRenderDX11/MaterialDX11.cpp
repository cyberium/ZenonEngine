#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDevice* renderDevice, size_t Size)
	: MaterialImpl(renderDevice, Size)
{}

MaterialDX11::~MaterialDX11()
{}

void MaterialDX11::Bind(const ShaderMap& shaders) const
{
    MaterialImpl::Bind(shaders);

	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			BindForShader(shader.second.get());
		}
	}

    for (auto shader : m_Shaders)
    {
		shader.second->Bind();
		BindForShader(shader.second.get());
    }

#ifdef USE_STL_FOREACH
	);
#endif
}

void MaterialDX11::Unbind(const ShaderMap& shaders) const
{
	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			UnbindForShader(shader.second.get());
		}
	}
	else
	{
		for (auto shader : m_Shaders)
		{
			UnbindForShader(shader.second.get());
		}
	}

    MaterialImpl::Unbind(shaders);
}

void MaterialDX11::BindForShader(const Shader * shader) const
{
	const Shader* pShader = shader;
	if (pShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			std::shared_ptr<Texture> texture = textureIt.second;
			texture->Bind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
			samplerState->Bind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
		}

		ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
		if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
		{
			materialParameter.Set<ConstantBuffer>(m_pConstantBuffer.get());
			materialParameter.Bind();
		}
	}
}

void MaterialDX11::UnbindForShader(const Shader * shader) const
{
	const Shader* pShader = shader;
	if (pShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			std::shared_ptr<Texture> texture = textureIt.second;
			texture->UnBind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
			samplerState->UnBind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
		}

		ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
		if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
		{
			materialParameter.UnBind();
		}
	}
}
