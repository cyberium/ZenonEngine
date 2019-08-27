#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDevice* renderDevice, size_t Size)
	: MaterialImpl(renderDevice, Size)
{}

MaterialDX11::~MaterialDX11()
{}

void MaterialDX11::Bind() const
{
    MaterialImpl::Bind();

#ifdef USE_STL_FOREACH
	const auto& shaders = GetShaders();
	std::for_each(shaders.begin(), shaders.end(), [this] (const std::pair<Shader::ShaderType, std::shared_ptr<Shader>>& shader)
#else
    for (auto shader : m_Shaders)
#endif
    {
        std::shared_ptr<Shader> pShader = shader.second;
        if (pShader)
        {
            pShader->Bind();

            for (const auto& textureIt : m_Textures)
            {
                std::shared_ptr<Texture> texture = textureIt.second;
                texture->Bind((uint32_t)textureIt.first, pShader.get(), ShaderParameter::Type::Texture);
            }

            for (const auto& samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->Bind((uint32_t)samplerStateIt.first, pShader.get(), ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.Set<ConstantBuffer>(m_pConstantBuffer.get());
                materialParameter.Bind();
            }
        }
    }

#ifdef USE_STL_FOREACH
	);
#endif
}

void MaterialDX11::Unbind() const
{
#ifdef USE_STL_FOREACH
	const auto& shaders = GetShaders();
	std::for_each(shaders.begin(), shaders.end(), [this](const std::pair<Shader::ShaderType, std::shared_ptr<Shader>>& shader)
#else
	for (auto shader : m_Shaders)
#endif
    {
        std::shared_ptr<Shader> pShader = shader.second;
        if (pShader)
        {
            for (const auto& textureIt : m_Textures)
            {
                std::shared_ptr<Texture> texture = textureIt.second;
                texture->UnBind((uint32_t)textureIt.first, pShader.get(), ShaderParameter::Type::Texture);
            }

            for (const auto& samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->UnBind((uint32_t)samplerStateIt.first, pShader.get(), ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.UnBind();
            }

            pShader->UnBind();
        }
    }

#ifdef USE_STL_FOREACH
	);
#endif

    MaterialImpl::Unbind();
}