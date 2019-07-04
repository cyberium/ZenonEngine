#include "stdafx.h"

// General
#include "MaterialDX11.h"

MaterialDX11::MaterialDX11(IRenderDevice* renderDevice)
	: MaterialImpl(renderDevice)
{}

MaterialDX11::~MaterialDX11()
{}

void MaterialDX11::Bind() const
{
    MaterialImpl::Bind();

    for (auto shader : m_Shaders)
    {
        std::shared_ptr<Shader> pShader = shader.second;
        if (pShader)
        {
            pShader->Bind();

            for (auto textureIt : m_Textures)
            {
                std::shared_ptr<Texture> texture = textureIt.second;
                texture->Bind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
            }

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->Bind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.Set<ConstantBuffer>(m_pConstantBuffer);
                materialParameter.Bind();
            }
        }
    }
}

void MaterialDX11::Unbind() const
{
    for (auto shader : m_Shaders)
    {
        std::shared_ptr<Shader> pShader = shader.second;
        if (pShader)
        {
            for (auto textureIt : m_Textures)
            {
                std::shared_ptr<Texture> texture = textureIt.second;
                texture->UnBind((uint32_t)textureIt.first, pShader, ShaderParameter::Type::Texture);
            }

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<SamplerState> samplerState = samplerStateIt.second;
                samplerState->UnBind((uint32_t)samplerStateIt.first, pShader, ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.UnBind();
            }

            pShader->UnBind();
        }
    }

    MaterialImpl::Unbind();
}